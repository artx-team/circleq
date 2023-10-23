# CIRCLEQ

[![Build](https://img.shields.io/github/actions/workflow/status/pavelxdd/circleq/meson.yml?branch=master&style=flat)](https://github.com/pavelxdd/circleq/actions)
[![License](https://img.shields.io/github/license/pavelxdd/circleq?style=flat&color=blue)](https://github.com/pavelxdd/circleq/blob/master/UNLICENSE)

CIRCLEQ is a single header intrusive circular linked list.

It is inspired by `<sys/queue.h>` from `glibc` and `FreeBSD`.

````bash
meson setup build -Dprefix=/usr/local
meson install -Cbuild
````
