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
  %10 = getelementptr inbounds [12 x i8], [12 x i8]* @.str1, i32 0, i32 0
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %10)
  %12 = load i32, i32* %2
  %13 = load i32, i32* %3
  %14 = add i32 %12, %13
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_d, i32 0, i32 0), i32 %14)
  %16 = getelementptr inbounds [12 x i8], [12 x i8]* @.str2, i32 0, i32 0
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %16)
  %18 = load i32, i32* %2
  %19 = load i32, i32* %3
  %20 = sub i32 %18, %19
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_d, i32 0, i32 0), i32 %20)
  %22 = getelementptr inbounds [12 x i8], [12 x i8]* @.str3, i32 0, i32 0
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %22)
  %24 = load i32, i32* %2
  %25 = load i32, i32* %3
  %26 = mul i32 %24, %25
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_d, i32 0, i32 0), i32 %26)
  %28 = getelementptr inbounds [12 x i8], [12 x i8]* @.str4, i32 0, i32 0
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %28)
  %30 = load i32, i32* %2
  %31 = load i32, i32* %3
  %32 = sdiv i32 %30, %31
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_d, i32 0, i32 0), i32 %32)
  %34 = getelementptr inbounds [12 x i8], [12 x i8]* @.str5, i32 0, i32 0
  %35 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %34)
  %36 = load i32, i32* %2
  %37 = load i32, i32* %3
  %38 = srem i32 %36, %37
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_d, i32 0, i32 0), i32 %38)
  %40 = getelementptr inbounds [13 x i8], [13 x i8]* @.str6, i32 0, i32 0
  %41 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %40)
  %42 = load i32, i32* %2
  %43 = sub i32 0, %42
  %44 = load i32, i32* %3
  %45 = add i32 %43, %44
  %46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_d, i32 0, i32 0), i32 %45)
  %47 = getelementptr inbounds [15 x i8], [15 x i8]* @.str7, i32 0, i32 0
  %48 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_s, i32 0, i32 0), i8* %47)
  %49 = load i32, i32* %3
  %50 = load i32, i32* %2
  %51 = add i32 %49, %50
  %52 = sub i32 0, %51
  %53 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.fmtstr_d, i32 0, i32 0), i32 %52)
  br label %return
return:
  ret i32 0
}

@.str7 = private unnamed_addr constant [15 x i8] c"-(b+a) result:\00"
@.str6 = private unnamed_addr constant [13 x i8] c"-a+b result:\00"
@.str5 = private unnamed_addr constant [12 x i8] c"a%b result:\00"
@.str4 = private unnamed_addr constant [12 x i8] c"a/b result:\00"
@.str3 = private unnamed_addr constant [12 x i8] c"a*b result:\00"
@.str2 = private unnamed_addr constant [12 x i8] c"a-b result:\00"
@.str1 = private unnamed_addr constant [12 x i8] c"a+b result:\00"
