package project;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.NoSuchAlgorithmException;
import java.util.*;

import util.HashGeneration;

public class MerkleTree {

	private Node root;
	private String path;
	ArrayList<String> leafHash;
	private Scanner sc;

	public MerkleTree(String path) {
		this.setRoot(null);
		this.path = path;
		leafHash = this.createLeafHash();
		this.createTree(this.calculateLevel());
	}
	public MerkleTree(String path, boolean download) {
		this.setRoot(null);
		this.path = path;
		leafHash = this.createLeafHashForDownloads();
		this.createTree(this.calculateLevel());
	}
	public MerkleTree() {
		
	}
	public int calculateLevel() {

		int exponent = 0;
		int height = 0;
		while (Math.pow(2, exponent) < this.numberOfFiles()) {
			exponent++;
		}
		if (Math.pow(2, exponent - 1) == this.numberOfFiles())
			height = exponent;
		else
			height = exponent + 1;

		return height;
	}

	public int numberOfFiles() {
		try {
			sc = new Scanner(new File(this.path));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		int numOfLines = 0;
		while (sc.hasNextLine()) {
			String line = sc.nextLine();
			if (!line.equalsIgnoreCase(""))
				numOfLines++;
		}
		return numOfLines;
	}

	public ArrayList<String> createLeafHash() {
		ArrayList<String> leafHash = new ArrayList<>();
		try {
			sc = new Scanner(new File(this.path));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		while (sc.hasNextLine()) {
			String line = sc.nextLine();
			if (!line.equalsIgnoreCase(""))
				try {
					leafHash.add(HashGeneration.generateSHA256(new File(line)));
				} catch (NoSuchAlgorithmException | IOException e) {
					e.printStackTrace();
				}
		}
		return leafHash;
	}
	
	public ArrayList<String> createLeafHashForDownloads() {
		ArrayList<String> leafHash = new ArrayList<>();
		try {
			sc = new Scanner(new File(this.path));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		while (sc.hasNextLine()) {
			String line = sc.nextLine();
			if (!line.equalsIgnoreCase("")) {
				String splitPath = splitPathOfDownload(line);
				try {
					leafHash.add(HashGeneration.generateSHA256(new File("secondaryPart/data/split/"+splitPath)));
				} catch (NoSuchAlgorithmException | IOException e) {
					e.printStackTrace();
				}
			}
		}
		return leafHash;
	}
	
	public String splitPathOfDownload(String url) {
		int index = url.length()-1;
		int reverseCounter=1;
		int counter=0;
		while(counter !=1) {
			if(url.charAt(index)=='/')
				counter++;
			reverseCounter++;
			index--;
		}
		
		String fileName = url.substring(url.length() - reverseCounter);
		return fileName;
	}

	public void add() {
		this.root = add(this.root);
	}

	private Node add(Node root) {
		if (root == null)
			root = new Node("");
		else {
			root.setLeft(add(root.getLeft()));
			root.setRight(add(root.getRight()));
		}
		return root;
	}

	public void createTree(int height){
		for (int i = 0; i < height; i++)
			this.add();
		fillBottom(root);
		fillIntermediate(root);
		removeExtras(root);
	}

	public void fillBottom(Node root)  {
		if (root != null) {
			if (root.getLeft() == null && root.getRight() == null && leafHash.size() > 0) {
				root.setData(this.leafHash.remove(0));
			}
			fillBottom(root.getLeft());
			fillBottom(root.getRight());
		}
	}

	public void fillIntermediate(Node root)  {
		if (root.getLeft() != null && root.getRight() != null) {
			fillIntermediate(root.getLeft());
			fillIntermediate(root.getRight());
			if (!(root.getLeft().getData() == "" && root.getRight().getData() == "")) {
				try {
					root.setData(HashGeneration.generateSHA256(root.getLeft().getData() + root.getRight().getData()));
				} catch (NoSuchAlgorithmException | UnsupportedEncodingException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}

	public void removeExtras(Node root) {
		if (root.getLeft() != null && root.getRight() != null) {
			removeExtras(root.getLeft());
			removeExtras(root.getRight());
			if (root.getLeft().getData() == "" && root.getRight().getData() == "") {
				root.setLeft(null);
				root.setRight(null);
				root.setData("");
			}

		}
	}

	/**
	 * @return the root
	 */
	public Node getRoot() {
		return root;
	}

	/**
	 * @param root
	 *            the root to set
	 */
	public void setRoot(Node root) {
		this.root = root;
	}

	public void printSideways() {
		printSideways(root, "");
	}

	private void printSideways(Node root, String indent) {
		if (root != null) {
			printSideways(root.getRight(), indent + "    ");
			System.out.println(indent + root.getData());
			printSideways(root.getLeft(), indent + "    ");
		}
	}

	public static void print(Node root) {
		LinkedList<Node> queue = new LinkedList<Node>();
		Node p;

		queue.add(root);
		while (!queue.isEmpty()) {
			p = queue.remove();
			if (p.getLeft() != null)
				queue.add(p.getLeft());
			if (p.getRight() != null)
				queue.add(p.getRight());
			System.out.println(p.getData());
		}
	}

	public boolean checkAuthenticity(String metaPath)  {
		try {
			sc = new Scanner(new File(metaPath));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String metaRoot = sc.nextLine();
		if (root.getData().equals(metaRoot)) {
			return true;
		}
		return false;
	}

	public ArrayList<Stack<String>> findCorruptChunks(String metaPath)  {
		ArrayList<Stack<String>> result = new ArrayList<>();
		MerkleTree metaTree = null;
		metaTree = createMetaTree(metaPath);
		this.mirror(metaTree);
		metaTree.fillMetaTree(metaPath);
		ArrayList<String> metaBottom = metaTree.getMetaBottom();
		ArrayList<String> treeBottom = this.createLeafHash();

		ArrayList<String> corruptedHashes = new ArrayList<>();
		//ana agacta olup meta agacta olmayan taban node lar, ekler.
		for(int i=0;i<treeBottom.size();i++) {
			if(!metaBottom.contains(treeBottom.get(i))) {
				corruptedHashes.add(treeBottom.get(i));
			}
		}
		//tüm pathlerde bozulmus hashlerin oldugu pathleri bulur.
		ArrayList<ArrayList<String>> paths = null;
		paths = this.paths();
		for(int i=0;i<corruptedHashes.size();i++) {
			Stack<String> corruptedPath = new Stack<>();
			for(int j =0;j<paths.size();j++) {
				if(paths.get(j).contains(corruptedHashes.get(i))) {
					while(!paths.get(j).isEmpty())
						corruptedPath.add(paths.get(j).remove(0));
				}
			}
			result.add(corruptedPath);
		}
		return result;
	}
	
	public ArrayList<Stack<String>> findCorruptChunksForDownload(String metaPath) {
		ArrayList<Stack<String>> result = new ArrayList<>();
		MerkleTree metaTree = null;
		metaTree = createMetaTree(metaPath);
		this.mirror(metaTree);
		metaTree.fillMetaTree(metaPath);
		ArrayList<String> metaBottom = metaTree.getMetaBottom();
		ArrayList<String> treeBottom = this.createLeafHashForDownloads();

		ArrayList<String> corruptedHashes = new ArrayList<>();
		//ana agacta olup meta agacta olmayan taban node lar, ekler.
		for(int i=0;i<treeBottom.size();i++) {
			if(!metaBottom.contains(treeBottom.get(i))) {
				corruptedHashes.add(treeBottom.get(i));
			}
		}
		//tüm pathlerde bozulmus hashlerin oldugu pathleri bulur.
		ArrayList<ArrayList<String>> paths = this.paths();
		for(int i=0;i<corruptedHashes.size();i++) {
			Stack<String> corruptedPath = new Stack<>();
			for(int j =0;j<paths.size();j++) {
				if(paths.get(j).contains(corruptedHashes.get(i))) {
					while(!paths.get(j).isEmpty())
						corruptedPath.add(paths.get(j).remove(0));
				}
			}
			result.add(corruptedPath);
		}
		return result;
	}
	//meta tree'den bozuk nodelari kaldirir
	public void mirror(MerkleTree metaTree) {
		LinkedList<Node> queueT = new LinkedList<Node>();
		LinkedList<Node> queueMeta = new LinkedList<Node>();
		Node t, m;

		queueT.add(this.getRoot());
		queueMeta.add(metaTree.getRoot());
		while (!queueT.isEmpty()) {
			t = queueT.remove();
			m= queueMeta.remove();
			
			if(t.getRight()!=null && t.getRight().getData().equals("")) {
				m.setRight(null);

			}
			if(t.getLeft()!=null && t.getLeft().getData().equals("")) {
				m.setLeft(null);

			}
			
			if (t.getLeft() != null) {
				queueT.add(t.getLeft());
				queueMeta.add(m.getLeft());
			}
			if (t.getRight() != null) {
				queueT.add(t.getRight());
				queueMeta.add(m.getRight());
			}
		}
	}
	//meta tree için bos agaç olusturur.
	public MerkleTree createMetaTree(String metaPath)  {
		MerkleTree metaTree = new MerkleTree();
		
		for(int i=0;i<this.calculateLevel();i++) {
			metaTree.add();
		}

		return metaTree;
	}
	//meta tree ye meta dosyasindakileri doldurur.
	public void fillMetaTree(String path)  {
		Scanner metaSc = null;
		try {
			metaSc = new Scanner(new File(path));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		LinkedList<Node> queue = new LinkedList<Node>();
		Node p;

		queue.add(this.getRoot());
		while (!queue.isEmpty()) {
			p = queue.remove();
			if (p.getLeft() != null)
				queue.add(p.getLeft());
			if (p.getRight() != null)
				queue.add(p.getRight());
			if(metaSc.hasNextLine())
				p.setData(metaSc.nextLine());
		}
		metaSc.close();
	}
	
	//meta tree'nin tüm taban hashlarini döndürür
	public ArrayList<String> getMetaBottom(){
		ArrayList<String> result = new ArrayList<>();
		Node p;
		LinkedList<Node> queue = new LinkedList<Node>();

		queue.add(this.getRoot());
		while (!queue.isEmpty()) {
			p = queue.remove();
			if (p.getLeft() != null)
				queue.add(p.getLeft());
			if (p.getRight() != null)
				queue.add(p.getRight());
			if(p.getLeft()==null && p.getRight()==null && !p.getData().equals(""))
				result.add(p.getData());
		}
		
		return result;
		
	}
	
	//ana tree deki tüm pathleri çikarir
	public ArrayList<ArrayList<String>> paths() {
		LightBringer lb = null;
		lb = new LightBringer(this);
		lb.printPaths(this.getRoot());

		return lb.paths;
	}
}
