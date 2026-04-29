# 01 — `alarm-zero` 검증

## 1. 이 테스트가 검증하는 것
`timer_sleep(0)` 호출 시 스레드가 잠들지 않고 즉시 반환되는지 검증합니다.

## 2. 구현 필요 함수와 규칙
### `timer_sleep()`
- 위치: `pintos/devices/timer.c`
- 규칙 1: `ticks <= 0`이면 바로 반환한다.
- 규칙 2: 이 경우 `sleep_list` 등록과 `thread_block()`을 수행하지 않는다.

## 3. 실패 시 바로 확인할 것
- 0 tick 입력에서 block 경로로 들어가는지
- 반환 전에 상태 변화가 발생하는지

## 4. 재검증
- `alarm-zero` 단일 실행 후 통과 확인
