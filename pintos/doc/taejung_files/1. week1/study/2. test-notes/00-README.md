# Alarm Clock 학습 가이드

이 폴더는 Alarm Clock을 **기능 이해 -> 테스트 검증** 흐름으로 학습하기 위한 문서 모음입니다.

## 학습 원칙
- 먼저 `1. feature`에서 기능 흐름을 이해한다.
- 구현은 함수별 규칙 문장을 기준으로 진행한다.
- 마지막은 `2. testing`의 테스트별 문서로 검증한다.

## 권장 학습 순서
1. `1. feature/01-feature-overview-alarm-flow.md`
2. `1. feature/02-feature-sleep-entry.md`
3. `1. feature/03-feature-wakeup-target-management.md`
4. `1. feature/04-feature-wakeup-execution-on-tick.md`
5. `1. feature/05-feature-scheduler-integration.md`
6. `1. feature/06-feature-failure-patterns.md`
7. `1. feature/07-feature-validation-by-tests.md`
8. `2. testing/00-README.md`
9. `2. testing/01-alarm-zero.md` ~ `2. testing/05-alarm-priority.md`
8. 필요 시 템플릿 사용
   - `../1. note-templates/TEMPLATE-feature-note.md`
   - `../1. note-templates/TEMPLATE-testing-note.md`

## 이 문서들의 역할
- `1. feature/01`: 기능 단위 전체 개요
- `1. feature/02`: 잠들기 진입 기능
- `1. feature/03`: 깨움 대상 관리 기능
- `1. feature/04`: tick 기반 깨우기 실행 기능
- `1. feature/05`: 스케줄러 연계 기능
- `1. feature/06`: 자주 실패하는 패턴 정리
- `1. feature/07`: 테스트로 검증 닫는 기준
- `2. testing/00`: 테스트 검증 인덱스
- `2. testing/01~05`: 테스트별 구현/검증 문서
- `../1. note-templates/*`: 새 문서 작성용 템플릿
