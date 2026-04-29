# 05 — `priority-donate-*` 검증

## 1. 이 테스트가 검증하는 것
락 대기 상황에서 donation이 적용/전파/회수되어 priority inversion이 완화되는지 검증합니다.

대상 테스트:
- `priority-donate-one`
- `priority-donate-multiple`, `priority-donate-multiple2`
- `priority-donate-nest`, `priority-donate-chain`
- `priority-donate-sema`, `priority-donate-lower`

## 1-1. 통합된 각 테스트가 실제로 확인하는 것
### `priority-donate-one`
- 단일 lock 대기에서 기본 donation이 즉시 적용되는지 확인
- lock 해제 후 우선순위가 정상 복원되는지 확인

### `priority-donate-multiple`
- 한 스레드가 여러 donor를 받을 때 가장 높은 donation이 반영되는지 확인
- lock 해제 순서에 따라 donation 회수가 단계적으로 반영되는지 확인

### `priority-donate-multiple2`
- `multiple`과 유사한 조건에서 donor 실행/종료 순서가 달라도 결과가 일관적인지 확인
- 다중 donation 상태에서 스케줄링 순서가 priority 규칙을 유지하는지 확인

### `priority-donate-nest`
- 중첩 lock 대기에서 donation이 한 단계 이상 전파되는지 확인
- 중간 owner의 effective priority 갱신이 누락되지 않는지 확인

### `priority-donate-chain`
- 긴 대기 체인에서 donation이 연쇄적으로 끝까지 전달되는지 확인
- 체인 해소 시 priority가 단계적으로 원상 복귀되는지 확인

### `priority-donate-sema`
- donation 상황과 semaphore 대기가 함께 있을 때 우선순위 규칙이 깨지지 않는지 확인
- lock 경계와 semaphore wake 경계가 충돌 없이 동작하는지 확인

### `priority-donate-lower`
- donation이 걸린 상태에서 base priority를 낮춰도 effective priority가 유지되는지 확인
- donation 해제 후에만 lowered base priority가 반영되는지 확인

## 2. 구현 필요 함수와 규칙
### `lock_acquire()`
- 위치: `pintos/threads/synch.c`
- 규칙 1: lock owner에게 donation을 전달한다.
- 규칙 2: 체인 대기에서는 상위 owner까지 전파한다.

### `lock_release()`
- 위치: `pintos/threads/synch.c`
- 규칙 1: 해제한 lock 관련 donation을 회수하고 effective priority를 재계산한다.

### `thread_set_priority()`
- 위치: `pintos/threads/thread.c`
- 규칙 1: base priority 변경과 donation 적용 상태를 분리해 관리한다.

## 3. 실패 시 바로 확인할 것
- donation 체인이 중간에서 끊기는지
- lock 해제 후 priority 복원이 잘못되는지
- base/effective priority가 같은 필드에 섞여 있는지

## 4. 재검증
- `priority-donate-one` -> `priority-donate-multiple` -> `priority-donate-chain` 순으로 단일 확인
- 마지막에 `priority-donate-*` 묶음 재실행
