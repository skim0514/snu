import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.ArrayList;
import java.util.List;


public class Main {
    /**
     * Iterate through each line of input.
     */
    public static void main(String[] args) throws IOException {
        //InputStreamReader reader = new InputStreamReader(System.in, StandardCharsets.UTF_8);
        //BufferedReader in = new BufferedReader(reader);
//        //String line;
//        while ((line = in.readLine()) != null) {
//
//        }
        System.out.println(function("Sun", 23));


    }

    public static String function(String day, int num) {
        List<String> days = Arrays.asList("Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun");
        return days.get((days.indexOf(day) + num) % 7);
    }
}