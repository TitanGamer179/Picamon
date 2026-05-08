@.fmtstr_true = private unnamed_addr constant [6 x i8] c"true\0A\00"
@.fmtstr_false = private unnamed_addr constant [7 x i8] c"false\0A\00"
@.fmtstr_d = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@.fmtstr_g = private unnamed_addr constant [7 x i8] c"%.08F\0A\00"
@.fmtstr_s = private unnamed_addr constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)



define i32 @main(i32 %argc, i8** %argv) {
.label0:
  %0 = sub i32 %argc, 1
  %1 = getelementptr inbounds i8*, i8** %argv, i32 1
  %2 = alloca i32
  %3 = alloca i32
  %4 = getelementptr inbounds i8*, i8** %argv, i32 1
  %5 = load i8*, i8** %4
  %6 = call i32 @atoi(i8* %5)
  store i32 %6, i32* %2
  %7 = getelementptr inbounds i8*, i8** %argv, i32 2
  %8 = load i8*, i8** %7
  %9 = call i32 @atoi(i8* %8)
  store i32 %9, i32* %3
  %10 = getelementptr inbounds [19 x i8], [19 x i8]* @.str1, i32 0, i32 0
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %10)
  %12 = load i32, i32* %2
  %13 = load i32, i32* %2
  %14 = icmp eq i32 %12, %13
  br i1 %14, label %.label1, label %.label2
.label1:
  %15 = load i32, i32* %3
  %16 = load i32, i32* %3
  %17 = icmp eq i32 %15, %16
  br label %.label2
.label2:
  %18 = phi i1 [ false, %.label0 ], [ %17, %.label1 ]
  br i1 %18, label %.label3, label %.label4
.label3:
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.fmtstr_true, i32 0, i32 0))
  br label %.label5
.label4:
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.fmtstr_false, i32 0, i32 0))
  br label %.label5
.label5:
  %21 = getelementptr inbounds [19 x i8], [19 x i8]* @.str2, i32 0, i32 0
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %21)
  %23 = load i32, i32* %2
  %24 = load i32, i32* %2
  %25 = icmp eq i32 %23, %24
  br i1 %25, label %.label7, label %.label6
.label6:
  %26 = load i32, i32* %3
  %27 = load i32, i32* %3
  %28 = icmp ne i32 %26, %27
  br label %.label7
.label7:
  %29 = phi i1 [ true, %.label5 ], [ %28, %.label6 ]
  br i1 %29, label %.label8, label %.label9
.label8:
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.fmtstr_true, i32 0, i32 0))
  br label %.label10
.label9:
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.fmtstr_false, i32 0, i32 0))
  br label %.label10
.label10:
  %32 = getelementptr inbounds [16 x i8], [16 x i8]* @.str3, i32 0, i32 0
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %32)
  %34 = load i32, i32* %2
  %35 = load i32, i32* %2
  %36 = icmp eq i32 %34, %35
  %37 = xor i1 %36, true
  br i1 %37, label %.label11, label %.label12
.label11:
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.fmtstr_true, i32 0, i32 0))
  br label %.label13
.label12:
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.fmtstr_false, i32 0, i32 0))
  br label %.label13
.label13:
  %40 = getelementptr inbounds [16 x i8], [16 x i8]* @.str4, i32 0, i32 0
  %41 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %40)
  %42 = load i32, i32* %3
  %43 = load i32, i32* %2
  %44 = icmp eq i32 %42, %43
  %45 = xor i1 %44, true
  br i1 %45, label %.label14, label %.label15
.label14:
  %46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.fmtstr_true, i32 0, i32 0))
  br label %.label16
.label15:
  %47 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.fmtstr_false, i32 0, i32 0))
  br label %.label16
.label16:
  br label %return
return:
  ret i32 0
}

@.str4 = private unnamed_addr constant [16 x i8] c"!(b==a) result:\00"
@.str3 = private unnamed_addr constant [16 x i8] c"!(a==a) result:\00"
@.str2 = private unnamed_addr constant [19 x i8] c"a==a||b!=b result:\00"
@.str1 = private unnamed_addr constant [19 x i8] c"a==a&&b==b result:\00"
