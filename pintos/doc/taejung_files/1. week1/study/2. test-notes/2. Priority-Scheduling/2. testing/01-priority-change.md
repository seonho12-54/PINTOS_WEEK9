# 01 — `priority-change` 검증

## 1. 이 테스트가 검증하는 것
현재 실행 스레드의 priority를 변경했을 때 스케줄러가 즉시 재평가되는지 검증합니다.

## 1-1. 이 테스트가 실제로 확인하는 것
- 고우선순위 보조 스레드가 먼저 실행/종료되고, 그 다음 현재 스레드가 계속 진행되는지 로그 순서를 확인한다.
- `thread_set_priority()` 호출 직후 현재 스레드가 계속 CPU를 점유하지 않고 필요 시 즉시 양보하는지 확인한다.
- donation이 없는 기본 상황에서 `thread_get_priority()`가 기대 priority를 반환하는지 확인한다.

## 2. 구현 필요 함수와 규칙
### `thread_set_priority()`
- 위치: `pintos/threads/thread.c`
- 규칙 1: 현재 스레드의 base priority를 갱신한다.
- 규칙 2: 변경 후 더 높은 READY 스레드가 있으면 `thread_yield()`로 양보한다.

### `thread_get_priority()`
- 위치: `pintos/threads/thread.c`
- 규칙 1: 테스트가 기대하는 effective priority 값을 일관되게 반환한다.

## 3. 실패 시 바로 확인할 것
- priority 값은 바뀌었는데 양보가 일어나지 않는지
- donation 상태에서 base/effective 구분이 섞여 있는지

## 4. 재검증
- `priority-change` 단일 실행 후 통과 확인
