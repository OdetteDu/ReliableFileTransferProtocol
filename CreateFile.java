import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.io.Writer;


public class CreateFile {
	
	public CreateFile(int bytesNo, int kbNo, int mbNo,int gbNo,int tbNo,int pbNo,int ppbNo){
		new genFile(bytesNo, kbNo, mbNo,gbNo, tbNo, pbNo, ppbNo);
	}
	
	public static void main(String[] args) {
		if (args.length == 0) {
			System.out.println("Flags: #bytes #KB #MB #GB");
			return;
		}
		int bytesNo = 0;
		int kbNo = 0;
		int mbNo = 0;
		int gbNo = 0;
		if (args.length > 0)
			bytesNo = Integer.valueOf(args[0]);
		if (args.length > 1)
			kbNo = Integer.valueOf(args[1]);
		if (args.length > 2)
			mbNo = Integer.valueOf(args[2]);
		if (args.length > 3)
			gbNo = Integer.valueOf(args[3]);
		//int tbNo = Integer.valueOf(args[4]);
		//int pbNo = Integer.valueOf(args[5]);
		//int ppbNo = Integer.valueOf(args[6]);
		new CreateFile(bytesNo, kbNo, mbNo, gbNo, 0, 0, 0);
		
		
	}

	public class genFile{
		
		int count = 0;
		
		public genFile(int bytesNo, int kbNo, int mbNo, int gbNo, int tbNo, int pbNo, int ppbNo){
			Writer writer = null;
			try {
				writer = new BufferedWriter(new OutputStreamWriter(
				      new FileOutputStream("./Size_"+ bytesNo+"b+" + kbNo + "KB+" +  mbNo+ "MB+" + gbNo + "GB"), "utf-8"));
				this.genPPB(ppbNo, writer);
				this.genPB(pbNo, writer);
				this.genTB(tbNo, writer);
				this.genGB(gbNo, writer);
				this.genMB(mbNo, writer);
				this.genKB(kbNo, writer);
				this.genByte(bytesNo, writer);
				
				System.out.println(count);
				try {
					writer.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				try {
					writer.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		public void genByte(int bytesNo, Writer writer){
			try {
			    int temp = bytesNo/26;
			    int more = bytesNo%26;
			    String seq = "abcdefghijklmnopqrstuvwxyz";
			    while(temp > 0){
			    	writer.write(seq);
			    	temp--;
			    }
			    writer.write(seq.substring(0, more));
			    
			} catch (IOException ex) {
			}
			count ++;
		}
		
		public void genKB(int kbNo, Writer writer){
			
			int temp = kbNo;
			while(temp > 0){
				this.genByte(1024, writer);
				temp --;
			}
		}
		
		public void genMB(int mbNo, Writer writer){
			int temp = mbNo;
			while(temp > 0){
				this.genKB(1024, writer);
				temp --;
			}
		}
		
		public void genGB(int gbNo, Writer writer){
			int temp = gbNo;
			while(temp > 0){
				this.genMB(1024, writer);
				temp --;
			}
		}
		
		public void genTB(int tbNo, Writer writer){
			int temp = tbNo;
			while(temp > 0){
				this.genGB(1024, writer);
				temp --;
			}
		}
		
		public void genPB(int pbNo, Writer writer){
			int temp = pbNo;
			while(temp > 0){
				this.genTB(1024, writer);
				temp --;
			}
		}
		
		public void genPPB(int ppbNo, Writer writer){
			int temp = ppbNo;
			while(temp > 0){
				this.genPB(1024, writer);
				temp --;
			}
		}
	}
}
