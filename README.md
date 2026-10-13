# Text Editor (C)

A line-based text editor written in C, split into a reusable editor library and a small interactive front end.

## What's inside

- **editor.c** — the interactive program.
- **editor_lib.c / editor_lib.h** — the editing engine (line storage, insertion, deletion, and file handling).
- **editor_lib_test.c** — unit tests for the library.
- **letter_to_editor.txt** — sample text to edit.

## Build & run

```sh
make            # builds editor and editor_lib_test
./editor
```

## Tech

C, Make
