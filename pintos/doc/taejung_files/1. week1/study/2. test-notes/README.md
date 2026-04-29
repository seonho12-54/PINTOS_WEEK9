# Threads 테스트 노트 가이드

이 폴더는 `Threads` 주차 문서를 **기능 이해 -> 구현 -> 테스트 검증** 흐름으로 정리한 진입 가이드입니다.

## 트랙 구성
- Alarm Clock 트랙: `1. Alarm-Clock`
- Priority Scheduling 트랙: `1. Priority-Scheduling`

## 공통 템플릿/생성 규칙
- 템플릿 위치: `../1. note-templates`
- 트랙별 생성 가이드:
  - `1. Alarm-Clock/00-template-and-generation-guide.md`
  - `1. Priority-Scheduling/00-template-and-generation-guide.md`

## 권장 학습 순서
1. Alarm Clock 트랙
   - `1. Alarm-Clock/1. feature/01-feature-overview-alarm-flow.md`
   - `1. Alarm-Clock/1. feature/02-feature-sleep-entry.md`
   - `1. Alarm-Clock/1. feature/03-feature-wakeup-execution-on-tick.md`
   - `1. Alarm-Clock/1. feature/04-feature-scheduler-integration.md`
   - `1. Alarm-Clock/2. testing/00-README.md`
   - `1. Alarm-Clock/2. testing/01-alarm-zero.md` ~ `05-alarm-priority.md`
2. Priority Scheduling 트랙
   - `1. Priority-Scheduling/1. feature/01-feature-overview-priority-flow.md`
   - `1. Priority-Scheduling/1. feature/02-feature-ready-queue-ordering.md`
   - `1. Priority-Scheduling/1. feature/03-feature-preemption-triggers.md`
   - `1. Priority-Scheduling/1. feature/04-feature-donation-and-sync-boundary.md`
   - `1. Priority-Scheduling/2. testing/00-README.md`
   - `1. Priority-Scheduling/2. testing/01-priority-change.md` ~ `05-priority-donate.md`

## 폴더 역할
- `1. Alarm-Clock`: Alarm 관련 feature/testing 문서 세트
- `1. Priority-Scheduling`: Priority 관련 feature/testing 문서 세트
- `../1. note-templates/*`: 새 문서 작성용 템플릿
