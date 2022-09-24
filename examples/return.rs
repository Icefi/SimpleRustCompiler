fn ret() {
    return;
}

fn func1(x:i32) -> i32 {
    return x;
}

fn main() {
    let a = 4;
    let b = 5 + 5;
    let c = (a + b) * 3;
    func1(c)
}
