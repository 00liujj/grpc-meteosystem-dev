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

  void CheckWorkStatus(const std::string& work) {
      WorkRequest request;
      request.set_workname(work);

      WorkReply reply;

      ClientContext context;

      CompletionQueue cq;
      std::unique_ptr<ClientAsyncReader<WorkReply> > reader(
          stub_->AsyncCheckWorkStatus(&context, request,&cq,(void*)1));
      void* got_tag;
      bool ok;

      bool ret = cq.Next(&got_tag, &ok);

      if (ret && ok && got_tag == (void*)1) {
          while (1) {
              reader->Read(&reply,(void*)1);
              ok = false;
              ret = cq.Next(&got_tag, &ok);
              if (!ret || !ok || got_tag != (void*)1) {
                  break;
              }
              std::cout << reply.workname() << " "
                  << reply.jobstatus() << " "
		  << reply.percent() << std::endl;
          }
      }
  }

  void GetData(const std::string& work) {
      DataRequest request;
      request.set_workname(work);

      DataReply reply;

      ClientContext context;

      CompletionQueue cq;
      std::unique_ptr<ClientAsyncReader<DataReply> > reader(
          datastub_->AsyncGetData(&context, request,&cq,(void*)2));
      void* got_tag;
      bool ok;

      bool ret = cq.Next(&got_tag, &ok);

      if (ret && ok && got_tag == (void*)2) {
          while (1) {
              reader->Read(&reply,(void*)2);
              ok = false;
              ret = cq.Next(&got_tag, &ok);
              if (!ret || !ok || got_tag != (void*)2) {
                  break;
              }

          }
          std::cout << work << "result: "
              << reply.data() << std::endl;
      }
  }

private:
  std::unique_ptr<Checker::Stub> stub_;
  std::unique_ptr<Data::Stub> datastub_;
};

int main(int argc, char** argv) {
    CheckerClient client(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));

    std::string work(" dojob");
    std::string work2(" doanotherjob");

    std::thread t1(&CheckerClient::GetData, &client, work);
    std::thread t2(&CheckerClient::CheckWorkStatus, &client, work);

    t1.join();
    t2.join();
    return 0;
}
