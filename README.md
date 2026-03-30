# SHML (SHell Markup Language) C library

## INTRO

the purpose of this C library is automatisation
written in under 400 lines, has few functoin but
the functionality is big

**requirements**
* C99 or later (GCC or TCC recommendet)
* standart C libraries

---

## ABI

### void Cleanup()
use it to avod memory leaks, just please use it

### int SHML(char* sml)
use it to run the SHML itself

**example:**
> `char* code = `
> `"?py:python '%s'?\n"`
> `"<py>print(15)</py>";`
> `SHML(code);`
> (SHML executes the code)

### void DefTag(char* tag,char* fmt)
use it to create a tag to then use (%s must be used) 

**example:**
> `DefTag("py", "python -c '%s'");`
> (yes the tags can be overwritten)

### int GetTagIndex(char* tag)
use it to get the index where the tag stays in the Stack[]

**example:**
> `DefTag("py", "python -c '%s'");`
> `int id = GetTagIndex("py");`
> (id = 0)

### char* WrapTag(char* text, char* tag)
use it to wrap the text using the tag

**example:**
> `DefTag("py", "python -c '%s'");`
> `WrapTag("print(15)". "py")`
> (id = "python -c 'print(15)'")

---

## SYNTAX

the syntax is like a Standart Markup Language
but with some changes:
**it doesnt have premade tags**
**no nested tags**
**depends mostly on your shell syntax**

### creating tags
creating tags is very simple
but you have to be specific

**example:**
> `?py:python -c '%s'?`

the **:** is the start of the format
the **?** is both the start of the definition
but also the end of the fomrat string and the definition
**%s** is where the text will be place

### using tags
using the tags is just like a Standart Markup Language 
would do: wrap the input between **<tagename>** and **</tagename>**

**example**
> `?py:python -c '%s'?`
> `<py>print(15)</py>`

your shell should print 15 if you have python installed

---
