# 02 — `alarm-negative` 검증

## 1. 이 테스트가 검증하는 것
`timer_sleep(음수)` 입력이 들어와도 스레드를 잠재우지 않고 즉시 반환되는지 검증합니다.

## 2. 구현 필요 함수와 규칙
### `timer_sleep()`
- 위치: `pintos/devices/timer.c`
- 규칙 1: `ticks < 0`도 `ticks <= 0` 조건으로 동일하게 처리한다.
- 규칙 2: 음수 입력에서 `wakeup_tick` 계산/등록/`thread_block()`을 수행하지 않는다.

## 3. 실패 시 바로 확인할 것
- 음수 입력이 정상 경로로 들어가 `sleep_list`에 등록되는지
- 입력 가드가 `ticks == 0`만 다루고 있는지

## 4. 재검증
- `alarm-negative` 단일 실행 후 통과 확인
