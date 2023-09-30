#pragma once

using u8 = unsigned char;
using i8 = signed char;

using u16 = unsigned short;
using i16 = signed short;

using u32 = unsigned long;
using i32 = signed long;

using u64 = unsigned long long;
using i64 = signed long long;

using f32 = float;
using f64 = double;



/* ASSERTIONS */

static_assert(sizeof(u8) == 1, "u8 must be an unsigned 8bit integer!");
static_assert(sizeof(i8) == 1, "i8 must be an signed 8bit integer!");

static_assert(sizeof(u16) == 2, "u16 must be an unsigned 16bit integer!");
static_assert(sizeof(i16) == 2, "i16 must be an signed 16bit integer!");

static_assert(sizeof(u32) == 4, "u32 must be an unsigned 32bit integer!");
static_assert(sizeof(i32) == 4, "i32 must be an signed 32bit integer!");

static_assert(sizeof(u64) == 8, "u64 must be an unsigned 64bit integer!");
static_assert(sizeof(i64) == 8, "i64 must be an signed 64bit integer!");

static_assert(sizeof(f32) == 4, "f32 must be an 32bit floating point number!");
static_assert(sizeof(f64) == 8, "f64 must be an 64bit floating point number!");

