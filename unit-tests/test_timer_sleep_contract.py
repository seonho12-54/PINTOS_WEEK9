import pathlib
import unittest


class TimerSleepContractTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        repo_root = pathlib.Path(__file__).resolve().parents[1]
        cls.timer_c_path = repo_root / "pintos" / "devices" / "timer.c"
        cls.timer_c = cls.timer_c_path.read_text(encoding="utf-8")

    def test_timer_sleep_has_non_positive_guard(self):
        self.assertRegex(
            self.timer_c,
            r"timer_sleep\s*\([^)]*\)\s*\{[\s\S]*?if\s*\(\s*ticks\s*<=\s*0\s*\)\s*\{[\s\S]*?return\s*;",
            "timer_sleep()에서 ticks <= 0 즉시 반환 가드가 필요합니다.",
        )

    def test_timer_sleep_uses_ordered_insert(self):
        self.assertIn(
            "list_insert_ordered(&sleep_list, &cur->elem, thread_compare_wakeup, NULL);",
            self.timer_c,
            "timer_sleep()은 sleep_list 정렬 삽입을 사용해야 합니다.",
        )

if __name__ == "__main__":
    unittest.main()
