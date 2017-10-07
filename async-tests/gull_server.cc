/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <memory>
#include <iostream>
#include <string>
#include <thread>

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

#include "gull.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;
using grpc::WriteOptions;
using gull::WorkRequest;
using gull::WorkReply;
using gull::DataRequest;
using gull::DataReply;
using gull::Checker;
using gull::Data;

class CallDataBase {
public:
  CallDataBase (ServerCompletionQueue* cq) : cq_(cq), status_(CREATE) {
  }

  virtual void Proceed(int & counter, std::string & donejob, bool & done) = 0;

  ServerCompletionQueue* cq_;

  enum CallStatus { CREATE, PROCESS, FINISH, DESTROY};
  CallStatus status_;
};

class CheckerServer final {
public:
  ~CheckerServer() {
      server_->Shutdown();
      cq_->Shutdown();
  }

  void Run() {
      std::string server_address("0.0.0.0:50051");

      ServerBuilder builder;
      builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
      builder.RegisterService(&service_);
      builder.RegisterService(&dataservice_);
      cq_ = builder.AddCompletionQueue();
      server_ = builder.BuildAndStart();
      std::cout << "Server listening on " << server_address << std::endl;

      HandleRpcs();
  }

private:
  class CallDataProgress : public CallDataBase {
  public:
    int stuff = 0;
    std::string stuff2 = "";
    bool stuff3 = false;

    CallDataProgress(Checker::AsyncService* service, ServerCompletionQueue* cq)
        : CallDataBase(cq), service_(service), responder_(&ctx_) {
        Proceed(stuff,stuff2,stuff3);
    }

    void Proceed(int & counter, std::string & donejob, bool & done) {
        if (status_ == CREATE) {
            status_ = PROCESS;
            service_->RequestCheckWorkStatus(&ctx_, &request_, &responder_, cq_, cq_, this);
      } else if (status_ == PROCESS) {
            new CallDataProgress(service_, cq_);

            donejob = donejob + std::to_string(counter);
            std::cout << donejob << std::endl;

            reply_.set_workname(request_.workname());
            reply_.set_jobstatus("PROCESS");
            reply_.set_percent(std::to_string(((double)counter / 3)*100) + "%");

            responder_.Write(reply_,this);
            counter++;
            status_ = (counter > 2) ? FINISH : PROCESS;
	    done = false;
      } else if (status_ == FINISH) {
            reply_.set_jobstatus("FINISH");
            reply_.set_percent(std::to_string(((double)counter / 3)*100) + "%");

            responder_.WriteAndFinish(reply_,WriteOptions(),Status::OK,this);
            status_ = DESTROY;
      } else {
            GPR_ASSERT(status_ == DESTROY);
            counter = 0;
	    done = true;
            delete this;
      }
    }

   private:
     Checker::AsyncService* service_;
     ServerContext ctx_;

     WorkRequest request_;
     WorkReply reply_;

     ServerAsyncWriter<WorkReply> responder_;
  };

  class CallDataGet : public CallDataBase {
  public:
    int stuff = 0;
    std::string stuff2 = "";
    bool stuff3 = false;

    CallDataGet(Data::AsyncService* service, ServerCompletionQueue* cq)
        : CallDataBase(cq), service2_(service), responder2_(&ctx2_) {
        Proceed(stuff,stuff2,stuff3);
    }

    void Proceed(int & counter, std::string & donejob, bool & done) {
        if (status_ == CREATE) {
            status_ = PROCESS;
            service2_->RequestGetData(&ctx2_, &request2_, &responder2_, cq_, cq_, this);
      } else if (status_ == PROCESS) {
            new CallDataGet(service2_, cq_);
            if (done) { 
                reply2_.set_data(donejob);
                status_ = FINISH;
	    } else {
		status_ = PROCESS;	
	    }
            responder2_.Write(reply2_,this);
      } else if (status_ == FINISH) {
            responder2_.Finish(Status::OK,this);
            status_ = DESTROY;
      } else {
            GPR_ASSERT(status_ == DESTROY);
            done = false;
            delete this;
      }
    }

   private:
     Data::AsyncService* service2_;
     ServerContext ctx2_;

     DataRequest request2_;
     DataReply reply2_;

     ServerAsyncWriter<DataReply> responder2_;
  };

  void HandleRpcs() {
      new CallDataProgress(&service_, cq_.get());
      new CallDataGet(&dataservice_, cq_.get());
      void* tag;
      bool ok;
      while (true) {
          GPR_ASSERT(cq_->Next(&tag, &ok));
          GPR_ASSERT(ok);
          static_cast<CallDataBase*>(tag)->Proceed(counter, donejob, done);
      }
  }

  std::unique_ptr<ServerCompletionQueue> cq_;
  Checker::AsyncService service_;
  Data::AsyncService dataservice_;
  std::unique_ptr<Server> server_;

  int counter = 0;
  std::string donejob;
  bool done = false;
};

int main(int argc, char** argv) {
    CheckerServer server;
    server.Run();

    return 0;
}
