# 05b · ISR bindings (§5)

Slides 23 (S-34: Board.h for both boards, lines 14–21) and 28 (S-36, right after the first build: typos are build errors now).

One handler per board, bound with `IsrBind`. Handlers are plain C++ functions in `bsp/<board>/Handlers.cpp`: mangled names, no `extern "C"`.

Error demos (each preset is expected to fail; `build.log` is the artifact):
- `-typo-enum`: misspelled enumerator (`TlM2`, `TIMER0`) → compile error (A-12)
- `-typo-handler`: misspelled handler name in Board.h → compile error (A-13)
- `-typo-definition`: Board.h is right, the definition in Handlers.cpp is misspelled → **link** error (A-14)
