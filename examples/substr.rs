fn substr(string: &str, sub: &str) -> i32 {

    let mstr = string.as_bytes();
    let sstr = sub.as_bytes();

    let mstr_len = mstr.len();
    let sstr_len = sstr.len();

    for i in 0..(mstr_len - sstr_len + 2) {
        let mut j = 0;

        while j < sstr_len {

            if mstr[i + j] != sstr[j] {
                break;
            }

            j += 1;

        }

        if j == sstr_len {
            return i as i32;
        }
    }

    return -1;
}
