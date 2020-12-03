//taken from "https://stackoverflow.com/questions/20265740/how-to-download-a-pdf-from-a-given-url-in-java?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa"
package project;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Stack;


public class Downloader {

	private String downloadPath;
	private ArrayList<String> fileNames = new ArrayList<>();

	public Downloader(String downloadPath) throws IOException {
		this.downloadPath = downloadPath;
		createFolderPath("secondaryPart/data/split/");
	}

	public void createInternalFiles() {
		Scanner sc = null;
		try {
			sc = new Scanner(new File(this.downloadPath));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		while (sc.hasNextLine()) {
			String url = sc.nextLine();
			if (url.equals("")) {
				continue;
			}
			String fileName = createFileName(url);
			if (!fileName.contains("alt") && !fileName.contains("meta")) {
				fileNames.add(fileName);
				createFolderPath("secondaryPart/data/split/" + fileName);
			}
			try {
				save(url, "secondaryPart/data/" + fileName + ".txt");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public void createSplitFiles() {
		for (int i = 0; i < fileNames.size(); i++) {
			Scanner sc = null;
			try {
				sc = new Scanner(new File("secondaryPart/data/" + fileNames.get(i) + ".txt"));
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			while (sc.hasNextLine()) {
				String url = sc.nextLine();
				if (url.equals("")) {
					continue;
				}
				String fileName = createSplitFileName(url);
				try {
					save(url, "secondaryPart/data/split/" + fileNames.get(i) + "/" + fileName);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			MerkleTree firstSource = new MerkleTree("secondaryPart/data/" + fileNames.get(i) + ".txt", true);
			boolean valid = firstSource.checkAuthenticity("secondaryPart/data/" + fileNames.get(i) + "meta.txt");
			if (valid == false) {
				ArrayList<Stack<String>> corrupts = firstSource.findCorruptChunksForDownload("secondaryPart/data/" + fileNames.get(i) + "meta.txt");
				int numOfCorruptedChunks = corrupts.size();
				for(int j=0;j<numOfCorruptedChunks;j++) {
					int indexOfCorruptedHash=firstSource.createLeafHashForDownloads().indexOf(corrupts.get(j).peek());
					Scanner deletingSc = null;
					try {
						deletingSc = new Scanner(new File("secondaryPart/data/" + fileNames.get(i) + ".txt"));
					} catch (FileNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					String stringOfCorruptedHash="";
					while(deletingSc.hasNextLine()) {
						String index = createSplitFileName(deletingSc.nextLine());
						if(indexOfCorruptedHash == Integer.parseInt(index))
							stringOfCorruptedHash=index;
					}
					File deletedFile = new File("secondaryPart/data/split/" + fileNames.get(i) + "/" + stringOfCorruptedHash);
					deletedFile.delete();
					Scanner altSc = null;
					try {
						altSc = new Scanner(new File("secondaryPart/data/" + fileNames.get(i) + "alt.txt"));
					} catch (FileNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					while (altSc.hasNextLine()) {
						String url = altSc.nextLine();
						if (url.equals("")) {
							continue;
						}
						String fileName = createSplitFileName(url);
						int fileInt = Integer.parseInt(fileName);
						if(fileInt == indexOfCorruptedHash) {
							try {
								save(url, "secondaryPart/data/split/" + fileNames.get(i) + "/" + fileName);
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
						}
					}
				}
			}
		}
	}

	// taken from
	// "https://stackoverflow.com/questions/20265740/how-to-download-a-pdf-from-a-given-url-in-java?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa"
	public void save(String source, String targetPath) throws IOException {
		URL url = new URL(source);
		InputStream in = url.openStream();
		Files.copy(in, Paths.get(targetPath), StandardCopyOption.REPLACE_EXISTING);
		in.close();
	}
	
	public String createFileName(String url) {
		String fileName = "";
		int index = 50;
		while (url.charAt(index) != '.') {
			fileName = fileName + url.charAt(index);
			index++;
		}
		return fileName;
	}

	public String createSplitFileName(String url) {
		int index = url.length()-1;
		int reverseCounter=1;
		while(url.charAt(index)!='/') {
			reverseCounter++;
			index--;
		}
		
		String fileName = url.substring(url.length() - reverseCounter+1);
		return fileName;
	}

	public void createFolderPath(String folderPath) {
		File file = new File(folderPath);
		file.mkdir();
	}

}
