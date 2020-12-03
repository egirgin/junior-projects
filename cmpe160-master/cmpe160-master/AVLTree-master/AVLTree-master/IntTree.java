package bts;

import java.util.NoSuchElementException;

public class IntTree {
	private IntTreeNode overallRoot;

	/**
	 * @return the overallRoot
	 */
	public IntTreeNode getOverallRoot() {
		return overallRoot;
	}

	/**
	 * @param overallRoot
	 *            the overallRoot to set
	 */
	public void setOverallRoot(IntTreeNode overallRoot) {
		this.overallRoot = overallRoot;
	}

	public void add(int value) {
		overallRoot = add(overallRoot, value);
		overallRoot = balance(overallRoot);
	}

	private IntTreeNode add(IntTreeNode root, int value) {
		if (root == null) {
			root = new IntTreeNode(value);
		} else if (root.data > value) {
			root.left = add(root.left, value);
			root.left = balance(root.left);
		} else if (root.data < value) {
			root.right = add(root.right, value);
			root.right = balance(root.right);
		} // else a duplicate

		return root;	
	}

	public boolean contains(int value) {
		return contains(overallRoot, value);
	}

	private boolean contains(IntTreeNode root, int value) {
		if (root == null) {
			return false;
		} else if (root.data == value) {
			return true;
		} else if (root.data > value) {
			return contains(root.left, value);
		} else { // root.data < value
			return contains(root.right, value);
		}
	}

	public void remove(int value) {
		overallRoot = remove(overallRoot, value);
	}

	private IntTreeNode remove(IntTreeNode root, int value) {
		if (root == null) {
			return null;
		} else if (root.data > value) {
			root.left = remove(root.left, value);
		} else if (root.data < value) {
			root.right = remove(root.right, value);
		} else { // root.data == value; remove this node
			if (root.right == null) {
				return root.left; // no R child; replace w/ L
			} else if (root.left == null) {
				return root.right; // no L child; replace w/ R
			} else {
				// both children; replace w/ min from R
				root.data = getMin(root.right);
				root.right = remove(root.right, root.data);
			}
		}
		return root;
	}

	public int getMin() {
		if (overallRoot == null) {
			throw new NoSuchElementException();
		}
		return getMin(overallRoot);
	}

	private int getMin(IntTreeNode root) {
		if (root.left == null) {
			return root.data;
		} else {
			return getMin(root.left);
		}
	}

	public int balanceFactor(IntTreeNode root) {
		return maxDepth(root.left) - maxDepth(root.right);
	}

	private int maxDepth(IntTreeNode node) {
		if (node == null) {
			return 0;
		} else {
			/* compute the depth of each subtree */
			int lDepth = maxDepth(node.left);
			int rDepth = maxDepth(node.right);

			/* use the larger one */
			if (lDepth > rDepth)
				return (lDepth + 1);
			else
				return (rDepth + 1);
		}
	}

	public IntTreeNode rotateRight(IntTreeNode root) {

		IntTreeNode leftright = root.left.right;

		IntTreeNode newRoot = root.left;

		newRoot.right = root;

		newRoot.right.left = leftright;

		return newRoot;

	}

	public IntTreeNode rotateLeft(IntTreeNode root) {
		IntTreeNode rightLeft = root.right.left;

		IntTreeNode newRoot = root.right;

		newRoot.left = root;

		newRoot.left.right = rightLeft;

		return newRoot;
	}

	public IntTreeNode balance(IntTreeNode root) {
		int bf = balanceFactor(root);
		if (bf < -1) {
			if (balanceFactor(root.right) < 0) {
				root = rotateLeft(root);
			} else if (balanceFactor(root.right) > 0) {
				root.right = rotateRight(root.right);
				root = rotateLeft(root);
			}

		} else if (bf > 1) {
			if (balanceFactor(root.left) > 0) {
				root = rotateRight(root);
			} else if (balanceFactor(root.left) < 0) {
				root.left = rotateLeft(root.left);
				root = rotateRight(root);
			}
		}
		return root;
	}

	public void printSideways() {
		printSideways(overallRoot, "");
	}

	private void printSideways(IntTreeNode root, String indent) {
		if (root != null) {
			printSideways(root.right, indent + "    ");
			System.out.println(indent + root.data);
			printSideways(root.left, indent + "    ");
		}
	}

}
