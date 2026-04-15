import os
import sys
import zipfile
import ftplib  # Assuming FTP network storage
from datetime import datetime


def send_zipped_logs(log_folder, archive_base_name, ftp_host, ftp_username, ftp_password, remote_path):
    """
    Zips log files from a folder and sends them to network file storage via FTP.

    Args:
            log_folder (str): Path to the folder containing log files.
            archive_base_name (str): Name of the archive file to create (without .zip extension).
            ftp_host (str): Hostname or IP address of the network file storage.
            ftp_username (str): Username for accessing the network file storage.
            ftp_password (str): Password for accessing the network file storage.
            remote_path (str): Path on the network storage where the archive will be uploaded.
    """
    # Get today's date in YYYY-MM-DD format
    now = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

    # Create archive filename with date and .zip extension
    archive_filename = f"{archive_base_name}_{now}.zip"
    try:
        # Create a ZIP file
        with zipfile.ZipFile(archive_filename, "w") as zip_file:
            for filename in os.listdir(log_folder):
                # Ignore non-file entries
                if not os.path.isfile(os.path.join(log_folder, filename)):
                    continue
                # Add each log file to the ZIP archive
                zip_file.write(os.path.join(log_folder, filename), filename)

        # Connect to FTP server
        ftp = ftplib.FTP(ftp_host, ftp_username, ftp_password)

        # Change directory to remote path (create if it doesn't exist)
        try:
            ftp.cwd(remote_path)
        except ftplib.all_errors:
            ftp.mkd(remote_path)
            ftp.cwd(remote_path)

        # Upload the archive file
        with open(archive_filename, "rb") as file:
            ftp.storbinary("STOR " + archive_filename, file)

        # Close connections
        ftp.quit()
        os.remove(archive_filename)  # Remove local archive after upload

        print(f"Log archive '{archive_filename}' successfully sent to '{remote_path}'")
    except Exception as e:
        print(f"Error sending zipped logs: {e}")

# Get log_folder from command line argument if provided
if len(sys.argv) > 1:
    log_folder = sys.argv[1]
else:
    log_folder = "logs"

archive_name = "cnc_logs"
ftp_host = "wx6.nbwxgj.com"
ftp_username = "ftpuaser"
ftp_password = "ftpuaser"
remote_path = "/home/ftpuaser"

send_zipped_logs(log_folder, archive_name, ftp_host, ftp_username, ftp_password, remote_path)

#if __name__ == "__main__":
#    send_zipped_logs(log_folder, archive_name, ftp_host, ftp_username, ftp_password, remote_path)
