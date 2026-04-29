# 04 — `alarm-simultaneous` 검증

## 1. 이 테스트가 검증하는 것
같은 tick에 깨어나야 하는 여러 스레드를 누락 없이 처리하는지 검증합니다.

## 2. 구현 필요 함수와 규칙
### `timer_interrupt()`
- 위치: `pintos/devices/timer.c`
- 규칙 1: 한 인터럽트에서 조건 만족 대상 하나만이 아니라 연속 구간 전체를 처리한다.
- 규칙 2: `wakeup_tick <= 현재 tick` 조건을 만족하는 동안 반복한다.

### `thread_compare_wakeup()`
- 위치: `pintos/devices/timer.c`
- 규칙 1: 동일 tick 스레드가 존재해도 정렬 불변식이 깨지지 않아야 한다.

## 3. 실패 시 바로 확인할 것
- while 루프가 아닌 if 한 번만 수행하는지
- pop/unblock 순서가 일관되지 않은지

## 4. 재검증
- `alarm-simultaneous` 단일 실행 후 통과 확인
