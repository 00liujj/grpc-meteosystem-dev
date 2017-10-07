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

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

#include "gull.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using gull::WorkRequest;
using gull::WorkReply;
using gull::DataRequest;
using gull::DataReply;
using gull::Checker;
using gull::Data;

class CheckerClient {
public:
  CheckerClient(std::shared_ptr<Channel> channel)
      : stub_(Checker::NewStub(channel)), datastub_(Data::NewStub(channel)) {}

  void AsyncCheckWorkStatus(const std::string& work)
  {
      WorkRequest request;
      request.set_workname(work);
      progress_reader = stub_->AsyncCheckWorkStatus(&context2, request,&cq,(void*)1);
  }

  void AsyncGetData(const std::string& work)
  {
      DataRequest request;
      request.set_workname(work);

      data_reader = datastub_->AsyncGetData(&context, request,&cq,(void*)2);
  }

  void GetData()
  {
      void* got_tag;
      bool ok;

      while(bool ret = cq.Next(&got_tag, &ok))
      {
        if (!ret)
            break;

	if(got_tag == (void*)3)
	    continue;

        if (got_tag == (void*)1) {
            if(!ok)
            {
                progress_reader->Finish((Status*)&Status::OK, (void*)3);
                continue;
	    }

            progress_reader->Read(&wreply,(void*)1);
            std::cout << wreply.workname() << " "
                << wreply.jobstatus() << " "
                << wreply.percent() << std::endl;
	}
 	else if (got_tag == (void*)2) {
	    if(!ok)
	    {
                data_reader->Read(&dreply,(void*)4);
                std::cout << "result: "
                    << dreply.data() << std::endl;
                data_reader->Finish((Status*)&Status::OK, (void*)4);
                break;
            }

            data_reader->Read(&dreply,(void*)2);

        }
      }
  }

private:
  std::unique_ptr<Checker::Stub> stub_;
  std::unique_ptr<Data::Stub> datastub_;

  ClientContext context;
  ClientContext context2;
  CompletionQueue cq;

  std::unique_ptr<ClientAsyncReader<DataReply> > data_reader;
  std::unique_ptr<ClientAsyncReader<WorkReply> > progress_reader;

  WorkReply wreply;
  DataReply dreply;


};

int main(int argc, char** argv) {
    CheckerClient client(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));

    std::string work(" dojob");
    std::string work2(" doanotherjob");

    client.AsyncCheckWorkStatus(work);
    client.AsyncGetData(work);

    std::thread t1(&CheckerClient::GetData, &client);
    t1.join();

    return 0;
}
