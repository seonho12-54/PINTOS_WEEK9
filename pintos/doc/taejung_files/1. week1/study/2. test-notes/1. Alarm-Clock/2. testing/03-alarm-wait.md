# 03 — `alarm-wait` 검증 (`single`, `multiple`)

## 1. 이 테스트가 검증하는 것
sleep 등록된 스레드가 요청한 tick 이후에 깨어나고, 여러 스레드일 때도 누락 없이 깨워지는지 검증합니다.

## 2. 구현 필요 함수와 규칙
### `timer_sleep()`
- 위치: `pintos/devices/timer.c`
- 규칙 1: `wakeup_tick = timer_ticks() + ticks`를 정확히 계산한다.
- 규칙 2: 현재 스레드를 `sleep_list`에 정렬 삽입 후 `thread_block()`한다.

### `thread_compare_wakeup()`
- 위치: `pintos/devices/timer.c`
- 규칙 1: `wakeup_tick` 오름차순 정렬을 유지하는 비교를 반환한다.

### `timer_interrupt()`
- 위치: `pintos/devices/timer.c`
- 규칙 1: head 기준으로 wake 조건을 검사한다.
- 규칙 2: 조건 만족 스레드를 반복적으로 `thread_unblock()`한다.

## 3. 실패 시 바로 확인할 것
- `wakeup_tick` 계산 오차
- 정렬 삽입 누락/비교 함수 방향 반전
- interrupt에서 단일 대상만 깨우고 종료

## 4. 재검증
- `alarm-wait` 실행 후 `single`, `multiple` 모두 통과 확인
