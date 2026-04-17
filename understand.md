
# Understanding Go-Style Garbage Collection

## 1. What Garbage Collection Means
Garbage collection frees memory that the program can no longer reach.

If no live variable can reach an object, that object is garbage.

## 2. Your Current Project vs Go

### Your current collector
1. Stop-the-world mark and sweep
2. Conservative stack scanning
3. Single-threaded collection

### Go collector
1. Mostly concurrent collection
2. Tri-color marking (white, gray, black)
3. Write barrier for pointer updates
4. Short stop-the-world phases at start and termination

## 3. Tri-Color Model

1. White:
Object not discovered yet (candidate to free)

2. Gray:
Object discovered, but children not scanned yet

3. Black:
Object discovered and fully scanned

Correctness rule:
A black object must not point to a white object.

## 4. Why Write Barrier Exists
During concurrent GC, the application keeps changing pointers.
Without a write barrier, GC may miss new references and free live objects.

Write barrier ensures new references are tracked during marking.

## 5. Simplified Go GC Cycle

1. GC Start (short pause)
Collect roots: stacks, globals, runtime roots
Push roots to gray queue

2. Concurrent Mark
While gray queue not empty:
- pop gray object
- scan its pointers
- white child becomes gray
- current object becomes black

3. Mark Termination (short pause)
Finish remaining mark work safely

4. Sweep
Free white (unmarked) objects
Reset mark state for next cycle

## 6. Why Go GC Is Fast
1. Most work runs concurrently
2. Pauses are kept short
3. GC pacing follows heap growth target
4. Background workers share mark/sweep effort

## 7. How To Map This To Your Project

1. include/common.h
Add object color/state metadata

2. src/allocator/mark_queue.h and src/allocator/mark_queue.c
Gray queue implementation

3. src/allocator/barrier.h and src/allocator/barrier.c
Write barrier and remembered set

4. src/gc/gc.c
GC state machine:
- start
- mark
- termination
- sweep

5. src/main.c
Stress tests:
- live object survival
- dead object reclamation
- repeated cycle stability

## 8. Step-by-Step Roadmap
1. Keep current stop-the-world collector stable
2. Add tri-color state + gray queue
3. Add write barrier on pointer stores
4. Move marking to background thread
5. Keep short pauses only for phase boundaries
6. Add pacing/tuning after correctness

## 9. Key Takeaway
Go GC is concurrent tri-color tracing with write barriers and pacing.
Your current project is a good base, and you can evolve it in phases.
EOF