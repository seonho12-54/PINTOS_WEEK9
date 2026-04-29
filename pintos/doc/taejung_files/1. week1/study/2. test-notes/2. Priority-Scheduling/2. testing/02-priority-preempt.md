# 02 — `priority-preempt` 검증

## 1. 이 테스트가 검증하는 것
더 높은 priority 스레드가 READY가 되는 즉시 선점이 반영되는지 검증합니다.

## 1-1. 이 테스트가 실제로 확인하는 것
- 고우선순위 스레드 생성 직후 저우선순위 스레드 출력보다 먼저 고우선순위 반복 출력이 연속으로 나오는지 확인한다.
- `"The high-priority thread should have already completed."` 시점 전에 high 스레드가 이미 종료됐는지 확인한다.
- `thread_create()`/`thread_unblock()` 경로에서 preemption 연결 누락이 없는지 실행 순서로 검증한다.

## 2. 구현 필요 함수와 규칙
### `thread_unblock()`
- 위치: `pintos/threads/thread.c`
- 규칙 1: READY 삽입 시 priority 정렬을 유지한다.
- 규칙 2: 필요하면 선점 경로를 연결한다.

### `thread_create()`
- 위치: `pintos/threads/thread.c`
- 규칙 1: 생성된 고우선순위 스레드가 지연되지 않도록 preemption 판단을 연계한다.

## 3. 실패 시 바로 확인할 것
- ready queue는 정렬되어 있는데 실행 전환 타이밍이 늦는지
- 생성 경로에서 선점 판단이 누락되었는지

## 4. 재검증
- `priority-preempt` 단일 실행 후 통과 확인
