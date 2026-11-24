#!/usr/bin/env python3
"""Check that Hometrainer runs on the 'Run' page of a built website.

Usage: test_run_page.py <URL of the run page>

The WebAssembly version of Hometrainer must show its main menu,
whose background is a gradient from grey (top-left) to white (bottom-right),
without errors on the page.
"""
import io
import sys
import time

from PIL import Image
from playwright.sync_api import sync_playwright

START_TIMEOUT_SECS = 120
TOP_LEFT = (5, 5)  # Grey (#808080) when the menu is shown
BOTTOM_RIGHT = (550, 750)  # Nearly white when the menu is shown
MAX_DIFF = 24  # Per color channel, from 0 to 255


def is_close(color, expected):
    """Is the color close to the expected color?"""
    return all(abs(a - b) <= MAX_DIFF for a, b in zip(color, expected))


def main():
    url = sys.argv[1]
    errors = []
    with sync_playwright() as playwright:
        browser = playwright.chromium.launch()
        page = browser.new_page(viewport={"width": 1280, "height": 1300})
        page.on("pageerror", lambda error: errors.append(str(error)))
        page.on("console", lambda m: print(f"console: {m.text}"))
        page.goto(url)
        frame = page.locator("iframe")

        print("Waiting for Hometrainer to show up")
        deadline = time.time() + START_TIMEOUT_SECS
        shown = False
        while time.time() < deadline and not shown:
            shot = Image.open(io.BytesIO(frame.screenshot())).convert("RGB")
            top_left = shot.getpixel(TOP_LEFT)
            bottom_right = shot.getpixel(BOTTOM_RIGHT)
            print(f"Top-left: {top_left}, bottom-right: {bottom_right}")
            shown = (is_close(top_left, (128, 128, 128))
                     and min(bottom_right) > 200)
            time.sleep(1)
        shot.save("run_page.png")
        assert shown, "Hometrainer menu not shown at the expected spot"
        assert not errors, errors
        browser.close()
    print("OK: Hometrainer runs")


if __name__ == "__main__":
    main()
