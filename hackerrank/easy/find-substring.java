// https://www.hackerrank.com/challenges/find-substring

import java.lang.*;
import java.util.*;
import java.util.regex.*;

public class Solution {
    public static void main(String[] args) {
        final Scanner s = new Scanner(System.in);

        final int numSentences = Integer.valueOf(s.nextLine());
        final List<String> sentences = new ArrayList<String>(numSentences);
        for (int i = 0; i < numSentences; ++i)
            sentences.add(s.nextLine());

        final int numQueries = Integer.valueOf(s.nextLine());
        for (int i = 0; i < numQueries; ++i) {
            final String query = s.nextLine();

            int matches = 0;
            for (String sentence : sentences) {
                final Matcher matcher = Pattern.compile("\\w" + query + "\\w").matcher(sentence);
                while (matcher.find())
                    ++matches;
            }

            System.out.println(matches);
        }
    }
}
