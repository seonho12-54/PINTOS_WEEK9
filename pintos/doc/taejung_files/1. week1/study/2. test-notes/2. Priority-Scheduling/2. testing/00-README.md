# 2. testing — Priority Scheduling 테스트 검증 가이드

이 폴더는 Priority Scheduling 테스트별 검증 문서 모음입니다.  
실패한 테스트 이름에 맞는 문서를 바로 열어 구현 포인트를 확인하고 재검증합니다.

## 문서 목록 (권장 순서)
1. `01-priority-change.md`
2. `02-priority-preempt.md`
3. `03-priority-fifo.md`
4. `04-priority-sema.md`
5. `05-priority-donate.md`
6. `06-priority-condvar.md`

## 사용 방법
- 실패한 테스트 문서를 먼저 연다.
- `구현 필요 함수와 규칙`만 확인하고 코드 구현으로 들어간다.
- 해당 테스트를 먼저 재실행하고, 마지막에 priority 관련 전체 테스트를 확인한다.
