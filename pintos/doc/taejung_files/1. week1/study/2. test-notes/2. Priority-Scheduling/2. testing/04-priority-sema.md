# 04 — `priority-sema` 검증

## 1. 이 테스트가 검증하는 것
세마포어 대기열(waiters)에서 높은 priority 스레드가 먼저 깨어나는지 검증합니다.

## 1-1. 이 테스트가 실제로 확인하는 것
- `Thread priority 30`부터 `21`까지 wake 출력이 내림차순 priority 순서로 나타나는지 확인한다.
- 각 wake 사이에 `Back in main thread.`가 기대대로 끼어들며 signal/down-up 핸드셰이크가 깨지지 않는지 확인한다.
- `sema_up()`이 FIFO pop으로 동작해 낮은 priority가 먼저 깨는 회귀가 없는지 확인한다.

## 2. 구현 필요 함수와 규칙
### `sema_down()`, `sema_up()`
- 위치: `pintos/threads/synch.c`
- 규칙 1: waiters에서 깨울 대상을 priority 기준으로 선택한다.
- 규칙 2: 깨운 뒤 preemption 판단 경로를 연계한다.

### `thread_unblock()`
- 위치: `pintos/threads/thread.c`
- 규칙 1: 세마포어 경로에서 넘어온 스레드도 READY 삽입 정책을 동일하게 적용한다.

## 3. 실패 시 바로 확인할 것
- `sema_up()`이 단순 FIFO pop만 수행하는지
- unblock 이후 선점 연결이 누락됐는지

## 4. 재검증
- `priority-sema` 단일 실행 후 통과 확인
