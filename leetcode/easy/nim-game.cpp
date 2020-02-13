// https://leetcode.com/problems/nim-game/
//
// In the Nim game, we take turns against an opponent where we must take 1 to 3
// stones from a pile during each turn. Whoever takes the last stones during
// their turn wins. Given both players play perfectly, with us starting first,
// canWinNim() should return whether or not it is possible for us regardless of
// what choices our opponent makes.
//
// The recursive implementation for this problem is:
//
// return n <= 3
//        || !canWinNim(n - 1) || !canWinNim(n - 2) || !canWinNim(n - 3);
//
// However, even with dynamic programming + backtracking, this solution will
// not pass the test cases... IMO the test cases are much too strict - we need a
// constant time solution!
//
// With a bit of thinking (or by reading the question on leetcode), you'll
// realise that when n==4, it is impossible to win if your turn is first and
// your opponent plays perfectly; your opponent can win immediately on their
// turn as there will be 3, 2 or 1 stone(s) left. However, if n==5,6,7, you can
// take enough stones on your turn to force your opponent to have 4 stones
// remaining on their turn, guaranteeing a way for you to win on your next
// turn!
//
// Now, if n==8, no matter how many stones we take, there will be 5, 6 or 7
// stones left on our opponents turn, and given what we've just learned, there
// is a guaranteed way for our opponent to force us to have 4 stones next turn,
// losing us the game.
//
// Therefore, if the number of stones left is evenly divisible by 4, there is a
// guaranteed way for our opponent to win!
//
// Complexity: runtime O(1), space O(1).

class Solution {
public:
    bool canWinNim(const int n) const
    {
        return n % 4; // or n & 3 (!= 0)
    }
};
