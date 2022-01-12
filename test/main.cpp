//
// Created by didi on 2021/12/22.
//

#include <iostream>
#include "vodka/runtime/runtime.h"
int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "Hello, World!\n";
  bool signal = false;

  {
    vodka::RuntimeConfig config = {
        .thread_resuse_ = true,
        .micro_task_scope_ = false
    };
    vodka::Runtime *rt = new vodka::Runtime(config);
    rt->Initialize();

    rt->RunOnJSThread([&signal, &rt](){
      std::cout << "set timeout\n";
      auto id = rt->context()->SetInterval([]{
        std::cout << "timeout fire \n";
      }, 0);
      rt->context()->ClearInterval(id);
      std::cout << "set timeout after\n";
      signal = true;
      rt->Destroy();
    });
  }

  while(signal == false){
    std::cout << "waiting for signal \n";
  }

  while(true){

  }
  return 0;
}
