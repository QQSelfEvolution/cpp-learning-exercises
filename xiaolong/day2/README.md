# Batch File Renamer & Text Search Tool

A C++ command-line tool for batch file renaming and text search operations.

## Features

- Command-line argument parsing with getopt_long
- File system traversal (recursive optional)
- Regular expression matching
- Batch rename operations with preview
- Text search in files with line numbers

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### Batch Rename

```bash
./file_tools rename --path ./files --pattern "*.txt" --find "old" --replace "new"
```

### Preview Rename (Dry Run)

```bash
./file_tools rename --path ./files --pattern "*.txt" --find "old" --replace "new" --dry-run
```

### Text Search

```bash
./file_tools search --path ./files --pattern "search_term" --recursive
```

### Search with Regex

```bash
./file_tools search --path ./files --pattern "error[0-9]+" --regex --recursive
```

## Options

### Rename Command
- `--path, -p`: Directory path (required)
- `--pattern, -t`: File pattern to match (e.g., "*.txt")
- `--find, -f`: String to find
- `--replace, -r`: String to replace
- `--dry-run, -d`: Preview changes without renaming

### Search Command
- `--path, -p`: Directory path (required)
- `--pattern, -t`: Search pattern (required)
- `--regex, -e`: Use regular expression
- `--recursive, -R`: Search recursively
- `--ignore-case, -i`: Case insensitive search

## Examples

### Rename all .txt files
```bash
./file_tools rename -p ./docs -t "*.txt" -f "draft" -r "final"
```

### Search for function definitions
```bash
./file_tools search -p ./src -t "void.*()" -e -R
```

### Find all TODO comments
```bash
./file_tools search -p ./src -t "TODO" -i -R
```
