; Juc compiler - LLVM IR output
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)

@.fmt_int    = private constant [3 x i8] c"%d\00"
@.fmt_double = private constant [6 x i8] c"%.16e\00"
@.fmt_str    = private constant [3 x i8] c"%s\00"
@.fmt_true   = private constant [5 x i8] c"true\00"
@.fmt_false  = private constant [6 x i8] c"false\00"

@.str0 = private constant [10 x i8] c"Is pair: \00"
@.str1 = private constant [3 x i8] c"|\0A\00"
@.str2 = private constant [2 x i8] c"\0A\00"
define i32 @b.int(i32 %input_arg) {
  %input = alloca i32
  store i32 %input_arg, i32* %input
  %k = alloca i32
  store i32 0, i32* %k
  %i = alloca i32
  store i32 0, i32* %i
  %g = alloca i32
  store i32 0, i32* %g
  %is_pair = alloca i32
  store i32 0, i32* %is_pair
  %r1 = add i32 0, 0
  store i32 %r1, i32* %k
  %r2 = load i32, i32* %k
  %r3 = add i32 0, 0
  store i32 %r3, i32* %i
  %r4 = load i32, i32* %i
  %r5 = add i32 0, 0
  store i32 %r5, i32* %g
  %r6 = load i32, i32* %g
  %r7 = add i32 0, 0
  store i32 %r7, i32* %is_pair
  %r8 = load i32, i32* %is_pair
  br label %lbl1
lbl1:
  %r9 = load i32, i32* %k
  %r10 = add i32 0, 5
  %r11 = icmp slt i32 %r9, %r10
  br i1 %r11, label %lbl2, label %lbl3
lbl2:
  %r12 = load i32, i32* %k
  %r13 = add i32 0, 1
  %r14 = icmp eq i32 %r12, %r13
  br i1 %r14, label %lbl4, label %lbl5
lbl4:
  %r15 = load i32, i32* %input
  %r16 = add i32 0, 2
  %r17 = sub i32 %r15, %r16
  store i32 %r17, i32* %input
  %r18 = load i32, i32* %input
  %r19 = add i32 0, 0
  store i32 %r19, i32* %i
  %r20 = load i32, i32* %i
  %r21 = add i32 0, 0
  store i32 %r21, i32* %g
  %r22 = load i32, i32* %g
  br label %lbl7
lbl7:
  %r23 = load i32, i32* %i
  %r24 = add i32 0, 100
  %r25 = icmp slt i32 %r23, %r24
  br i1 %r25, label %lbl8, label %lbl9
lbl8:
  br label %lbl10
lbl10:
  %r26 = load i32, i32* %g
  %r27 = add i32 0, 50
  %r28 = icmp slt i32 %r26, %r27
  br i1 %r28, label %lbl11, label %lbl12
lbl11:
  %r29 = load i32, i32* %input
  %r30 = add i32 0, 2
  %r31 = add i32 %r29, %r30
  store i32 %r31, i32* %input
  %r32 = load i32, i32* %input
  %r33 = load i32, i32* %g
  %r34 = add i32 0, 2
  %r35 = srem i32 %r33, %r34
  %r36 = add i32 0, 0
  %r37 = icmp eq i32 %r35, %r36
  br i1 %r37, label %lbl13, label %lbl14
lbl13:
  %r38 = load i32, i32* %is_pair
  %r39 = add i32 0, 1
  %r40 = add i32 %r38, %r39
  store i32 %r40, i32* %is_pair
  %r41 = load i32, i32* %is_pair
  br label %lbl15
lbl14:
  br label %lbl15
lbl15:
  %r42 = load i32, i32* %g
  %r43 = add i32 0, 1
  %r44 = add i32 %r42, %r43
  store i32 %r44, i32* %g
  %r45 = load i32, i32* %g
  br label %lbl10
lbl12:
  %r46 = add i32 0, 0
  store i32 %r46, i32* %g
  %r47 = load i32, i32* %g
  %r48 = load i32, i32* %i
  %r49 = add i32 0, 1
  %r50 = add i32 %r48, %r49
  store i32 %r50, i32* %i
  %r51 = load i32, i32* %i
  br label %lbl7
lbl9:
  br label %lbl6
lbl5:
  %r52 = load i32, i32* %k
  %r53 = add i32 0, 2
  %r54 = icmp eq i32 %r52, %r53
  br i1 %r54, label %lbl16, label %lbl17
lbl16:
  %r55 = load i32, i32* %input
  %r56 = add i32 0, 3
  %r57 = mul i32 %r55, %r56
  store i32 %r57, i32* %input
  %r58 = load i32, i32* %input
  br label %lbl18
lbl17:
  br label %lbl18
lbl18:
  br label %lbl6
lbl6:
  %r59 = load i32, i32* %k
  %r60 = add i32 0, 1
  %r61 = add i32 %r59, %r60
  store i32 %r61, i32* %k
  %r62 = load i32, i32* %k
  br label %lbl1
lbl3:
  %r63 = getelementptr inbounds [10 x i8], [10 x i8]* @.str0, i32 0, i32 0
  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %r63)
  %r64 = load i32, i32* %is_pair
  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %r64)
  %r65 = getelementptr inbounds [3 x i8], [3 x i8]* @.str1, i32 0, i32 0
  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %r65)
  %r66 = load i32, i32* %input
  ret i32 %r66
lbl19:
  ret i32 0
}
define void @main.StringArray(i8** %args_arg, i32 %__len_args_arg) {
  %args = alloca i8**
  store i8** %args_arg, i8*** %args
  %__len_args = alloca i32
  store i32 %__len_args_arg, i32* %__len_args
  %aux1 = alloca i32
  store i32 0, i32* %aux1
  %r1 = add i32 0, 40
  store i32 %r1, i32* %aux1
  %r2 = load i32, i32* %aux1
  %r3 = load i32, i32* %aux1
  %r4 = call i32 @b.int(i32 %r3)
  store i32 %r4, i32* %aux1
  %r5 = load i32, i32* %aux1
  %r6 = add i32 0, 0
  %r7 = load i8**, i8*** %args
  %r8 = getelementptr inbounds i8*, i8** %r7, i32 %r6
  %r9 = load i8*, i8** %r8
  %r10 = call i32 @atoi(i8* %r9)
  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %r10)
  %r11 = getelementptr inbounds [2 x i8], [2 x i8]* @.str2, i32 0, i32 0
  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %r11)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  %__argc_minus_1 = sub i32 %argc, 1
  %__argv_plus_1 = getelementptr inbounds i8*, i8** %argv, i32 1
  call void @main.StringArray(i8** %__argv_plus_1, i32 %__argc_minus_1)
  ret i32 0
}

