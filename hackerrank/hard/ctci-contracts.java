// https://www.hackerrank.com/challenges/ctci-contracts

import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;

public class Solution {
    // cheese with hash tables
    public static void main(String[] args) {
        final Scanner in = new Scanner(System.in);
        final int n = in.nextInt();
        final Map<String, Integer> tries = new HashMap<String, Integer>();

        for(int a0 = 0; a0 < n; ++a0){
            final String op = in.next();
            final String value = in.next();

            if (op.equals("add")) {
                for (int i = 0; i < value.length(); ++i) {
                    final String trie = value.substring(0, i + 1);
                    final Integer trieCount = tries.get(trie);
                    tries.put(trie, Integer.valueOf(trieCount != null ? trieCount + 1 : 1));
                }
            } else if (op.equals("find")) {
                final Integer trieCount = tries.get(value);
                System.out.println(trieCount != null ? trieCount : Integer.valueOf(0));
            }
        }
    }
}
