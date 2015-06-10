import smtplib
import sys
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText

fromaddr = 'motionsensoralterau@gmail.com'
toaddr = 'abraham@abrahamsalloum.com'
subject = "ALERT"
body = "\nMotion has been detected on sensor " +str(sys.argv[1])+" click here for details http://heycitizen.com:8080/this.php"
msg = MIMEMultipart()
msg['From'] = fromaddr
msg['To'] = toaddr
msg['Subject'] = subject
msg.attach(MIMEText(body, 'plain')) 

username = 'motionsensoralertau@gmail.com'
password = 'salloum1'

server = smtplib.SMTP('smtp.gmail.com',587)
server.ehlo()
server.starttls()
server.ehlo()
server.login(username,password)
txt = msg.as_string()
server.sendmail(fromaddr, toaddr, txt)
server.quit()
