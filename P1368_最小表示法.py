from typing import List

def get_min_representation(s: List[int]) -> int:
    N = len(s)
    i = 0
    j = 1
    while i < N and j < N:
        k = 0
        while k < N and s[(i + k) % N] == s[(j + k) % N]:
            k += 1
        if k == N:
            break
        if s[(i + k) % N] < s[(j + k) % N]:
            j += k + 1
            if i == j:
                j += 1
        else:
            i += k + 1
            if i == j:
                i += 1
    return min(i, j)

def main():
    N = int(input())
    s = [int(x) for x in input().split()]
    ans = get_min_representation(s)

    for k in range(N):
        print("{} ".format(s[(ans + k) % N]), end="")


if __name__ == "__main__":
    main()
