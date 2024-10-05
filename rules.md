For functions, classes and constants please use PascalCase.
For variables and method names (class function members) please use snake_case.
For global constants, environment variables, macros please use UPPER_SNAKE_CASE.
For structures please use snake_case, and if it's a type, Please suffix it with "_t".
Please stay consistent through the sub-projects in terms of style.
Please use 4-space as tabulation in all of the code (libraries aren't included).
Please follow Single Reponsibilit Principle (SRP), a function must do one thing and do it well. If function cannot be split in to multiple, then keep it as is until the solution is found. Please aim for function size to be between 20-30 lines.
Please use descriptive names and don't abbrievate when not needed, some names can be abbrieviated like architecture-specific names (such as IDT in x86_64).
Please comment intelligently, don't over comment code and try make the code as readable as possible.
Please use DRY - Don't Repeat Yourself, avoid repeating code, if you find yourself copying and pasting code, consider refactoring it into reusable functions or classes.
Please use KISS - Keep It Simple, Stupid, do not overcomplicate or over-engineer a solution. Simplicity enhances readability and maintainability.
Please avoid global variables as it may introduce issues and make debugging harder.
Please handle errors gracefully, if something fails, don't kill process (or whole system) immediately, try finding a solution first, and all possibilities are exhausted, just kill the process.
Please write test cases for code if possible.
Please avoid magic numbers and strings if possible.
Please refactor when neccesary.
Please use meaningful constants when making ones.

When writing a kernel, please refrain from using OS-specific functions.