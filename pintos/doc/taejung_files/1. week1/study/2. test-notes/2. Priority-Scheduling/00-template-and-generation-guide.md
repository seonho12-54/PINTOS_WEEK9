# Priority Scheduling 테스트 노트 템플릿/양식/생성 가이드

이 문서는 Priority Scheduling 학습 문서를 새로 만들거나 확장할 때,  
같은 형식으로 빠르게 작성하기 위한 통합 가이드입니다.

---

## 1. 폴더 구조 기준

- 기능 문서: `1. Priority-Scheduling/1. feature`
- 테스트 문서: `1. Priority-Scheduling/2. testing`
- 공용 템플릿: `../../1. note-templates`

---

## 2. 현재 표준 문서 세트

### feature 문서
- `01-feature-overview-priority-flow.md`
- `02-feature-ready-queue-ordering.md`
- `03-feature-preemption-triggers.md`
- `04-feature-donation-and-sync-boundary.md`

### testing 문서
- `00-README.md`
- `01-priority-change.md`
- `02-priority-preempt.md`
- `03-priority-fifo.md`
- `04-priority-sema.md`
- `05-priority-donate.md`

---

## 3. 템플릿 사용 규칙

### 3.1 feature 문서 템플릿
- 기준: `../../1. note-templates/TEMPLATE-feature-note.md`
- 용도: 기능 단위 학습/구현 문서 작성
- 필수 섹션:
  - `1. 구현 목적 및 필요성`
  - `2. 가능한 구현 방식 비교`
  - `3. 시퀀스와 단계별 흐름`
  - `4. 구현 주석`
  - `5. 테스팅 방법`

### 3.2 testing 문서 템플릿
- 기준: `../../1. note-templates/TEMPLATE-testing-note.md`
- 용도: 테스트 하나 기준 검증 문서 작성
- 필수 섹션:
  - `1. 이 테스트가 검증하는 것`
  - `2. 구현 필요 함수와 규칙`
  - `3. 실패 시 바로 확인할 것`
  - `4. 재검증`

---

## 4. 구현 주석 작성 양식 (핵심 규칙)

구현 주석은 "바로 코드에 반영 가능한 정보"만 남깁니다.

- 기본 형식:
  - `위치: <파일 경로>`
  - `역할: <이 함수가 하는 일>`
  - `규칙 1, 규칙 2...: <구현 조건>`

- 포함 기준:
  - 이번 단계에서 실제로 수정/추가할 함수만 포함
  - Donation/Preemption처럼 경로가 여러 개인 경우 함수별로 분리해 기록

- 제외 기준:
  - 구현 위치/조건이 없는 추상 문장
  - 테스트 설명과 중복되는 일반론

---

## 5. 문서 생성 절차 (권장 워크플로우)

1. feature인지 testing인지 범위를 먼저 확정한다.
2. 해당 템플릿을 새 파일로 복사한다.
3. 구현 주석에 함수/위치/규칙을 구체화한다.
4. `2. testing/00-README.md`와 상위 `2. test-notes/00-README.md` 링크를 동기화한다.
5. priority 관련 단일 테스트를 먼저 실행하고, 마지막에 묶음 검증한다.

---

## 6. 빠른 체크리스트

- [ ] 문서 목적이 feature/testing 중 하나로 명확한가?
- [ ] 구현 주석에 위치/역할/규칙이 모두 있는가?
- [ ] preemption/donation 규칙이 함수 단위로 분해되어 있는가?
- [ ] 상위 README 링크와 번호가 최신인가?
- [ ] 관련 `priority-*` 테스트와 설명이 일치하는가?
