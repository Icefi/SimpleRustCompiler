pub fn nod(x: i32, y: i32) -> i32 {

    let mut a = x;
    let mut b = y;

    let result = loop {
        if a == 0 || b == 0 {
            break a + b;
        }

        if a > b {
            a = a % b;
        } else {
            b = b % a;
        }
    };

    result
}
