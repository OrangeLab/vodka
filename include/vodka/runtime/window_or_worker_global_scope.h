//
// Created by didi on 2021/12/30.
//

#ifndef VODKA_WINDOW_OR_WORKER_GLOBAL_SCOPE_H
#define VODKA_WINDOW_OR_WORKER_GLOBAL_SCOPE_H

/// https://developer.mozilla.org/zh-CN/docs/Web/API/WindowOrWorkerGlobalScope
namespace vodka {

static int setInterval(ScriptState*,
                       EventTarget&,
                       V8Function* handler,
                       int timeout,
                       const HeapVector<ScriptValue>&);
static int setInterval(ScriptState*,
                       EventTarget&,
                       const String& handler,
                       int timeout,
                       const HeapVector<ScriptValue>&);
static void clearTimeout(EventTarget&, int timeout_id);
static void clearInterval(EventTarget&, int timeout_id);

}
#endif  // VODKA_WINDOW_OR_WORKER_GLOBAL_SCOPE_H
