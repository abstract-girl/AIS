import mysql.connector
import json

mydb = mysql.connector.connect(
    host='127.0.0.1',
    user='stud',
    passwd='stud',
    database='archdb',
    port=6033
)

cursor = mydb.cursor()

cursor.execute('SELECT id FROM User')

ids = [id[0] for id in cursor.fetchall()]

with open('ids.json', 'w') as f:
    json.dump(ids, f)

