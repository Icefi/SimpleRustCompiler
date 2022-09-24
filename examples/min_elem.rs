pub fn find_min(arr: [i32; 5]) -> i32 {
    let mut min_elem = arr[0];

    for i in arr {
        if i < min_elem {
            min_elem = i;
        }
    }

    return min_elem;
}
