# 06 — `priority-condvar` 검증

## 1. 이 테스트가 검증하는 것
`cond_wait()`로 대기 중인 스레드가 `cond_signal()` 시점에 우선순위 기준으로 깨어나는지 검증합니다.

핵심은 "조건변수 waiters 선택 순서"가 priority 정책과 일치하는지입니다.

## 1-1. 이 테스트가 실제로 확인하는 것
- `Signaling...` 이후 wake 되는 스레드가 `30 -> 29 -> ... -> 21` 순으로 내려가는지 확인한다.
- start 순서와 wake 순서가 다를 수 있어도, wake는 반드시 priority 기준이어야 함을 확인한다.
- `cond_signal()`의 waiter 선택과 내부 `sema_up()` 선택이 모두 우선순위 규칙을 따르는지 검증한다.

## 2. 구현 필요 함수와 규칙
### `cond_wait()`
- 위치: `pintos/threads/synch.c`
- 규칙 1: waiter를 조건변수 waiters에 넣을 때, 해당 waiter가 내부적으로 sema 대기 우선순위를 반영할 수 있어야 한다.

### `cond_signal()`
- 위치: `pintos/threads/synch.c`
- 규칙 1: 깨울 waiter를 priority 기준으로 선택한다.
- 규칙 2: 선택된 waiter의 semaphore를 `sema_up()` 하여 READY 전이를 유도한다.

### `sema_up()`
- 위치: `pintos/threads/synch.c`
- 규칙 1: semaphore waiters에서도 highest priority를 우선 unblock 한다.
- 규칙 2: 필요 시 선점 경로를 연계한다.

## 3. 실패 시 바로 확인할 것
- `cond_signal()`이 waiters에서 임의/FIFO pop을 하고 있지 않은지
- condvar waiter 비교 기준이 실제 thread priority를 반영하는지
- `sema_up()`에서 highest priority unblock이 깨지지 않았는지

## 4. 재검증
- `priority-condvar` 단독 실행
- 이후 `priority-sema`, `priority-preempt`까지 연속 실행해 wake/선점 연계를 확인
