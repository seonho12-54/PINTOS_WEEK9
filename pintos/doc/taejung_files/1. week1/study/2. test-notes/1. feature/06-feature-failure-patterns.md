# 06 — Feature Failure Patterns

## 1. 구현 목적 및 필요성
### 이 기능이 무엇인가
테스트 실패를 원인 코드(F1~F5)로 빠르게 분류해 수정 지점을 즉시 찾게 해주는 복구 기준 문서입니다.

### 왜 이걸 하는가 (문제 맥락)
Alarm은 "거의 맞는 구현"에서 자주 실패합니다. 실패를 함수 단위로 좁히지 못하면 디버깅 시간이 크게 늘어납니다.

### 무엇을 연결하는가 (기술 맥락)
실패 테스트 -> 원인 코드(F1~F5) -> 수정 함수 -> 재검증 흐름을 고정합니다.

### 완성의 의미 (결과 관점)
테스트가 깨졌을 때 어디를 고칠지 바로 판단할 수 있습니다.

## 2. 가능한 복구 방식 비교
- 방식 A: 실패 로그를 그때그때 해석
  - 장점: 준비물 없음
  - 단점: 반복 실수에 약함
- 방식 B: 원인 코드 기반 복구
  - 장점: 재현/수정/검증이 빠름
  - 단점: 초기에 규칙 정리 필요
- 선택: B

## 3. 시퀀스와 단계별 흐름
```mermaid
flowchart LR
  A[테스트 실패] --> B[F코드 분류]
  B --> C[함수 수정]
  C --> D[단일 재검증]
  D --> E[전체 재검증]
```

시퀀스를 단계로 읽으면 다음과 같습니다.
1. 실패 테스트 이름을 확인한다.
2. F1~F5 중 패턴 코드를 결정한다.
3. 연결 함수만 먼저 수정한다.
4. 실패 테스트 단일 확인 후 전체 테스트를 돌린다.

## 4. 구현 주석 (패턴별 수정 함수)
### 4.1 F1: 입력 계약 누락
- 위치: `pintos/devices/timer.c` (`timer_sleep`)
- 역할: 잘못된 입력(`ticks <= 0`)을 정상 sleep 경로에서 즉시 차단한다.
- 규칙 1: `ticks <= 0` 입력은 즉시 반환한다.
- 규칙 2: 이 경로에서 `sleep_list` 등록/`thread_block`을 수행하지 않는다.

### 4.2 F2: 정렬 불변식 붕괴
- 위치: `pintos/devices/timer.c` (`thread_compare_wakeup`, `timer_sleep`)
- 역할: wakeup 순서가 깨지지 않도록 `sleep_list` 정렬 불변식을 유지한다.
- 규칙 1: 비교 기준은 `wakeup_tick`으로 고정한다.
- 규칙 2: 삽입 후에도 head가 최소 tick이어야 한다.

### 4.3 F3/F4: wake 루프 불완전
- 위치: `pintos/devices/timer.c` (`timer_interrupt`)
- 역할: 깨울 대상 누락 없이 연속 구간 전체를 안전하게 처리한다.
- 규칙 1: 조건 만족 대상을 1개가 아닌 연속 구간 전체 처리한다.
- 규칙 2: pop 후 unblock 순서를 일관되게 유지한다.

### 4.4 F5: 책임 경계 위반
- 위치: `pintos/devices/timer.c`, `pintos/threads/thread.c`
- 역할: Alarm 로직이 scheduler 책임을 침범하지 않도록 경계를 고정한다.
- 규칙 1: Alarm은 wake 판단 + unblock까지만 담당한다.
- 규칙 2: 실행 순서 결정은 scheduler에 위임한다.

## 5. 테스팅 방법
- F1 의심: `alarm-zero`, `alarm-negative`
- F2/F3/F4 의심: `alarm-wait`, `alarm-simultaneous`
- F5 의심: `alarm-priority`
