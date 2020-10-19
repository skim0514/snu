public class Calculate {
    private static double rem = 0.0;
    public static void calculateChange(double purchasePrice, double cash) {
        if (cash >= purchasePrice) {
            calculator(cash-purchasePrice);
        } else {
            System.out.println("ERROR");
        }
    }

    public static void calculator(double cash) {
        if (cash >= 50) {
            System.out.print("Fifty Pounds");
            rem = cash - 50.;
        } else if (cash >= 20) {
            System.out.print("Twenty Pounds");
            rem = cash - 20.;
        } else if (cash >= 10) {
            System.out.print("Ten Pounds");
            rem = cash - 10.;
        } else if (cash >= 5) {
            System.out.print("Five Pounds");
            rem = cash - 5.;
        } else if (cash >= 2) {
            System.out.print("Two Pounds");
            rem = cash - 2.;
        } else if (cash >= 1) {
            System.out.print("One Pound");
            rem = cash - 1.;
        } else if (cash >= .50) {
            System.out.print("Fifty Pence");
            rem = cash - .50;
        } else if (cash >= .2) {
            System.out.print("Twenty Pence");
            rem = cash - .2;
        } else if (cash >= .1) {
            System.out.print("Ten Pence");
            rem = cash - .10;
        } else if (cash >= .05) {
            System.out.print("Five Pence");
            rem = cash - .05;
        } else if (cash >= .02) {
            System.out.print("Two Pence");
            rem = cash - .02;
        } else if (cash >= .01) {
            System.out.print("One Pence");
            rem = cash - .01;
        }

        if (rem != 0) {
            System.out.print(", ");
            rem = (double) Math.round(rem * 100) / 100;
            calculator(rem);
        }
    }
}
