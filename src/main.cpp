#include <any/asm.h>
#include <any/scheduler.h>
#include <any/loader.h>
#include <any/actor.h>
#include <any/errno.h>
#include <any/std_io.h>
#include <any/std_string.h>

#include <iostream>

static void* myalloc(void*, void* old, aint_t sz)
{
    return realloc(old, (size_t)sz);
}

static void on_panic(aactor_t* a)
{
    aint_t ev_idx = any_top(a);
    if (any_type(a, ev_idx).type == AVT_STRING) {
        std::cout << "[" << ascheduler_pid(a->owner, a) << "] panic - " <<
            any_to_string(a, ev_idx) << "\n";
    }
    else {
        std::cout << "[" << ascheduler_pid(a->owner, a) << "] panic - " <<
            "unknown fatal error" << "\n";
    }
}

static void on_unresolved(const char* module, const char* name)
{
    std::cout << "unresolved import `" << module << ':' << name << "`\n";
}

int main()
{
    ascheduler_t s;
    aerror_t ec;

    enum { NUM_IDX_BITS = 4 };
    enum { NUM_GEN_BITS = 4 };

    ascheduler_init(&s, NUM_IDX_BITS, NUM_GEN_BITS, &myalloc, NULL);
    ascheduler_on_panic(&s, &on_panic);
    aloader_on_unresolved(&s.loader, &on_unresolved);

    astd_lib_add_io(
        &s.loader, [](void*, const char* str) { std::cout << str; }, NULL);
    astd_lib_add_string(&s.loader);

    aactor_t* a;
    ascheduler_new_actor(&s, 1024*32, &a);
    any_find(a, "std-io", "print/*");
    any_push_string(a, "that's string");
    ascheduler_start(&s, a, 1);
    ascheduler_run_once(&s);

    ascheduler_cleanup(&s);
    return 0;
}