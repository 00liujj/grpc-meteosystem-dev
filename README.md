# grpc-meteosystem-dev

## WhatToDo
    cd ./grpc-meteosystem-dev && make

## Updates

List of'em.

### Update 1

Update of [async-client-server](async-tests) : [cервер](async-tests/gull_server.cc) с двумя асинхронными сервисами принимает удаленные [вызовы](async-tests/gull.proto) от асинхронных заглушек одного [клиента](async-tests/gull_client.cc) на получение результата вычислений и подписку на стрим с прогрессом выполнения оных.
