package main;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Stack;
import project.Downloader;
import project.MerkleTree;

public class Main {

	public static void main(String[] args) {

		MerkleTree m0 = new MerkleTree("data/1_bad.txt");
		String hash = m0.getRoot().getLeft().getRight().getData();
		System.out.println(hash);

		boolean valid = m0.checkAuthenticity("data/0meta.txt");
		System.out.println(valid);

		// The following just is an example for you to see the usage.
		// Although there is none in reality, assume that there are two corrupt
		// chunks in this example.
		ArrayList<Stack<String>> corrupts = m0.findCorruptChunks("data/1meta.txt");
		System.out.println("Corrupt hash of first corrupt chunk is: " + corrupts.get(0).pop());
		try {
			System.out.println("Corrupt hash of second corrupt chunk is: " + corrupts.get(1).pop());
		} catch (IndexOutOfBoundsException e) {
			
		}

		download("secondaryPart/data/download_from_trusted.txt");

	}

	public static void download(String path) {
		Downloader dw = null;
		try {
			dw = new Downloader(path);
		} catch (IOException e) {
			e.printStackTrace();
		}
		dw.createInternalFiles();
		dw.createSplitFiles();
	}

}
