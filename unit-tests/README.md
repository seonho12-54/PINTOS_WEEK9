# Unit Tests (CI/Local)

이 폴더는 Pintos 내부 테스트(`pintos/tests/...`)와 별개인 단위 테스트 모음입니다.

## 목적
- 구현 계약(Contract)을 빠르게 검증
- CI/로컬에서 동일한 방식으로 실행
- OS 의존 최소화 (macOS/Windows 공통)

## 실행 방법

### 공통 (권장)
```bash
python3 unit-tests/run_local.py
```

### macOS/Linux
```bash
bash unit-tests/run_local.sh
```

### Windows (PowerShell)
```powershell
powershell -ExecutionPolicy Bypass -File unit-tests/run_local.ps1
```

## 현재 포함된 테스트
- `test_timer_sleep_contract.py`
  - `timer_sleep` 입력 가드(`ticks <= 0`)
  - `sleep_list` 정렬 삽입 사용 여부

## 범위 원칙
- 단위테스트는 **현재 구현 완료된 항목만** 검증합니다.
- 미구현 항목(예: `timer_init` 초기화, `timer_interrupt` wake 루프)은 구현 PR에서 테스트를 확장합니다.
