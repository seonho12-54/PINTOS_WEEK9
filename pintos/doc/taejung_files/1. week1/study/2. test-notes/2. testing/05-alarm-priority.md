# 05 — `alarm-priority` 검증

## 1. 이 테스트가 검증하는 것
Alarm wake-up 이후 READY 상태 전이와 우선순위 스케줄링 규칙이 충돌 없이 동작하는지 검증합니다.

## 2. 구현 필요 함수와 규칙
### `thread_unblock()`
- 위치: `pintos/threads/thread.c`
- 규칙 1: 깨운 스레드를 `READY`로 전이한다.
- 규칙 2: ready queue 삽입 시 우선순위 정책을 깨지 않는다.

### `timer_interrupt()`
- 위치: `pintos/devices/timer.c`
- 규칙 1: wake 판단과 unblock까지만 담당하고 실행 순서를 직접 결정하지 않는다.

## 3. 실패 시 바로 확인할 것
- Alarm 코드에서 실행 순서까지 직접 건드리는 로직이 있는지
- ready queue 삽입 정책이 priority 규칙과 충돌하는지

## 4. 재검증
- `alarm-priority` 단일 실행 후 통과 확인
