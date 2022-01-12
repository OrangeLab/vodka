//
// Created by didi on 2022/1/10.
//

#include "vodka/runtime/runtime.h"
#include "vodka/js_task_runner.h"
namespace vodka {

Runtime::Runtime(RuntimeConfig config):config_(config){}

void Runtime::Initialize() {

  // Runtime 在主线程 堆上构建，JS 线程析构， 因此捕获 this 为安全引用
  std::function<void()> initailizeWorker = [this](){
    if (this->is_destory_){
      return;
    }
    context_ = JSContext::Create(this->task_runner_);
  };
  std::shared_ptr<JSTask> task = std::make_shared<JSTask>(std::move(initailizeWorker));

  if (config_.thread_resuse_){
    task_runner_ = GlobalJSTaskRunner::GetInstance();
    task_runner_->PostTask(task);
  }else{
    task_runner_ = std::make_shared<JSTaskRunner>();
    task_runner_->PostTask(task);
  }
}

void Runtime::RunOnJSThread(std::function<void()> &&callback) {
  if (is_destory_){return;}
  if (config_.micro_task_scope_){

    task_runner_->PostTask(std::make_shared<JSTask>(std::move(callback)));
  } else{
    task_runner_->PostTask(std::make_shared<JSTask>(std::move(callback)));
  }
}

void Runtime::Destroy() {
  is_destory_ = true;
  // JS 线程析构
  std::function<void()> finalizer = [handle = std::shared_ptr<Runtime>(this)](){};
  task_runner_->PostTask(std::make_shared<JSTask>(std::move(finalizer)));
}
}