# grpc-meteosystem-dev

## WhatToDo
    cd ./grpc-meteosystem-dev && make

## Updates

List of'em.

### Update 1

Update of [async-client-server](async-tests) : [cервер](async-tests/gull_server.cc) с двумя асинхронными сервисами принимает удаленные [вызовы](async-tests/gull.proto) от асинхронных заглушек одного [клиента](async-tests/gull_client.cc) на получение результата вычислений и подписку на стрим с прогрессом выполнения оных.

### Update 2

Update of [async-client-server](async-test) : [клиент](async-tests/gull_client.cc): разные теги на разные службы и их этапы обработки, исправленная логика ответов серверу, вынесена отдельно; [cервер](async-tests/gull_server.cc): общая очередь обработки запросов.

### Update 3
Add [demo-files](demos) : здесь будут всякие наработки, в том числе и визуальные, подробнее - внутри.
