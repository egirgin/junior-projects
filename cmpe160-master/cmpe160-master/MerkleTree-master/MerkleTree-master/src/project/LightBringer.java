// taken from "https://www.geeksforgeeks.org/given-a-binary-tree-print-all-root-to-leaf-paths/"
package project;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;

public class LightBringer {
	int numOfLeaves;
	MerkleTree mt;
	ArrayList<ArrayList<String>> paths = new ArrayList<>();

	public LightBringer(MerkleTree tree)  {
		this.mt = tree;
		numOfLeaves = tree.leafHash.size();
//		for (int i = 0; i < numOfLeaves; i++) {
//			paths.add(new ArrayList<>());
//		}
	}

	void printPaths(Node node)  {
		String path[] = new String[mt.calculateLevel()];
		printPathsRecur(node, path, 0);
	}

	/*
	 * Recursive helper function -- given a node, and an array containing the path
	 * from the root node up to but not including this node, print out all the
	 * root-leaf paths.
	 */
	void printPathsRecur(Node node, String path[], int pathLen) {
		if (node == null)
			return;

		/* append this node to the path array */
		path[pathLen] = node.getData();
		pathLen++;

		/* it's a leaf, so print the path that led to here */
		if (node.getLeft() == null && node.getRight() == null)
			printArray(path, pathLen);
		else {
			/* otherwise try both subtrees */
			printPathsRecur(node.getLeft(), path, pathLen);
			printPathsRecur(node.getRight(), path, pathLen);
		}
	}

	/* Utility function that prints out an array on a line. */
	void printArray(String ints[], int len) {
		ArrayList<String> path = new ArrayList<>();
		int i;
        for (i = 0; i < len; i++) 
        {
           path.add(ints[i]);
        }        	
        paths.add(path);
        if(path.contains("aaa"))
        	paths.remove(path);
	}

}
