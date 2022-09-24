fn fibonacci(a: i32) -> i32 {
    let fib_sum = 0;

    let fib1 = 1;
    let fib2 = 1;

    let j = 0;

    while j < a - 2 {
        fib_sum = fib1 + fib2;
        fib1 = fib2;
        fib2 = fib_sum;
        j = j + 1;
    }

    return fib2;
}

fn factorial(n: i32) -> i32 {

    let i = 1;

    let result = 1;

    while i < n {
        i = i + 1;
        result = result * i;
    }

    return result;
}

fn main() -> i32 {

    let x = 0;

    print("Input x\n");

    scan("%d", x);

    let fact = factorial(x);
    let fibb = fibonacci(x);

    print("Factorial is %d\n", fact);
    print("Fibonacci is %d\n", fibb);

    return x;
}
