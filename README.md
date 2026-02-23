*This project has been created as part of the 42 curiculum by martafer.*

# get_next_line

## Description

The purpose of **get_next_line** is to implement a function capable of reading a file descriptor line by line.

The main goal of this project is to deeply understand:
- Static variables
- File descriptors
- Dynamic memory allocation
- Buffer management
- Efficient string manipulation in C

The function `get_next_line(int fd)` reads from a given file descriptor and returns one line at a time, including the newline character (`\n`) if present. Each call returns the next line until the end of file (EOF) or an error occurs.

This project emphasizes memory management, avoiding leaks, and handling edge cases such as:
- Files without a trailing newline
- Empty files
- Large files
- Different `BUFFER_SIZE` values

---

## Instructions

### Compilation

Compile the project with the flags `-Wall -Wextra -Werror` and define `BUFFER_SIZE`:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c
```
Note that you may change the buffer size.

#### Usage Example
```c
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int	fd = open("1char.txt", O_RDONLY);
	char	*line;
	while (1)
	{
		line = get_next_line(fd);
		printf("%s\n", line);
		if (line == NULL)
			break;
		free(line);
	}
	free(line);
	close(fd);
	return (0);
}
```
---

## Algorithm Explanation and Justification

### Core Idea

The selected algorithm is based on maintaining a **static buffer (`stash`)** that preserves unread content between function calls.

Each call to `get_next_line` follows these steps:

1. **Read and accumulate data**  
   The function reads from the file descriptor into a temporary buffer of size `BUFFER_SIZE`.  
   The read content is appended to the static `stash` until a newline character (`\n`) is found or EOF is reached.

2. **Extract the next line**  
   Once a newline is detected (or no more data can be read), the function extracts the first complete line from `stash`.

3. **Clean the stash**  
   The extracted line is removed from `stash`, and the remaining content is preserved for the next call.

---

### Why This Algorithm?

This approach was chosen because:

- It ensures **no data is lost between calls** thanks to the static variable.
- It allows **incremental reading**, which is memory-efficient.
- It works correctly regardless of the `BUFFER_SIZE`.
- It avoids reading the entire file into memory.
- It properly handles edge cases like:
  - Partial reads
  - Files without trailing newline
  - Empty files
  - Read errors

The separation into helper functions improves clarity and modularity:

- `read_to_stash()` → Reads and accumulates data.
- `extract_line()` → Extracts the next full line.
- `clean_stash()` → Removes the extracted line from memory.
- `join_and_free()` → Safely concatenates buffers and handles memory management.

This modular design improves maintainability, readability, and debugging.

---

## Technical Decisions

- A **static variable** is used to persist data between calls.
- Memory is always freed when no longer needed to prevent leaks.
- The function handles `read()` returning:
  - `-1` → Error
  - `0` → EOF
- String operations rely on custom functions (`ft_strlen`, `ft_strdup`, `ft_strjoin`, `ft_strchr`).

---

## Features

- Reads one line per call
- Works with any valid file descriptor
- Supports different `BUFFER_SIZE` values
- Handles large files
- Clean modular structure

---

## Resources

### Documentation & References

- The Open Group Base Specifications — `read()` manual  
- Linux manual pages (`man 2 read`)  
- 42 Intranet subject for get_next_line  

### AI Usage

Artificial intelligence tools were used for:

- Improving documentation clarity and structure.
- Reviewing algorithm explanations for clarity.

AI was **not used to generate the core implementation logic**. The algorithm design, memory management decisions, and debugging process were developed manually as part of the learning objectives of the project.

---

## Conclusion

This project reinforces low-level file handling and memory management in C. It demonstrates how to manage persistent state across function calls and how to safely manipulate dynamically allocated memory.

It is a foundational exercise for understanding more advanced system programming concepts.
