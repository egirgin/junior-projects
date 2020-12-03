from flask import Flask, render_template, request, redirect, url_for
from flaskext.mysql import MySQL

app = Flask(__name__)
mysql = MySQL()

app.config['MYSQL_DATABASE_USER'] = 'root'
app.config['MYSQL_DATABASE_PASSWORD'] = '1e4m8r6e'
app.config['MYSQL_DATABASE_HOST'] = 'localhost'
app.config['MYSQL_DATABASE_DB'] = 'dbtify'


mysql.init_app(app)

conn = mysql.connect()

cursor = conn.cursor()

def create_DB():
    """
    if (cursor.execute("SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'dbtify' ") == 0):
        cursor.execute("CREATE DATABASE dbtify;")
        conn.commit()
        cursor.execute("USE dbtify;")
        conn.commit()
        app.config['MYSQL_DATABASE_DB'] = 'dbtify'
    """
    cursor.execute("CREATE DATABASE IF NOT EXISTS dbtify;")
    conn.commit()
    cursor.execute("USE dbtify;")
    conn.commit()

table_names = ["listener", "artist", "album", "song", "song_artist", "listener_song", "listener_album"]

def create_tables():
    for table_name in table_names:
        if (cursor.execute("SELECT * FROM information_schema.tables WHERE table_schema = '{}' AND table_name = '{}' LIMIT 1;".format(app.config['MYSQL_DATABASE_DB'], table_name)) == 0):
            with open("./procedures/{}.sql".format(table_name), "r") as table_create_query:
                cursor.execute(table_create_query.read())
                conn.commit()


def delete_DB():
    cursor.execute("DROP DATABASE IF EXISTS dbtify;")
    conn.commit()


@app.route("/")
def main():
    return redirect(url_for('usertype'))

@app.route('/initDB')
def initDB():
    #delete_DB()
    #create_DB()
    create_tables()
    return render_template("userType.html")

@app.route('/login')
def login():
    #get db username (typically root) and password and place into app.config
    pass


@app.route('/usertype')
def usertype():
    return render_template('userType.html')


@app.route('/artist', methods=["POST","GET"])
def artist():

    cursor.execute("SELECT * FROM dbtify.artist")
    
    artist_list = cursor.fetchall()
    
    name = ""
    surname = ""

    if request.method == "POST":
        name = request.form["name"]
        surname = request.form["surname"]

        cursor.execute( "SELECT EXISTS(SELECT * FROM dbtify.artist WHERE name = '{}' AND surname = '{}' )".format(name, surname) )

        if cursor.fetchone()[0] > 0: # check if artist is already exists
            return render_template('artistMainPage.html', artist_list = artist_list, name = name, surname=surname, duplicate = True)

        else: # if not exists in db then add it
            cursor.execute("INSERT INTO artist (name, surname) VALUES ('{}', '{}');".format(name, surname))
            conn.commit()
            #return all the artists from db
            cursor.execute("SELECT * FROM dbtify.artist")
            artist_list = cursor.fetchall()
            return render_template('artistMainPage.html', artist_list = artist_list, name=name, surname=surname, duplicate=False)

    
    return render_template('artistMainPage.html', artist_list = artist_list)
    

@app.route('/artistProfile', methods=["POST", "GET"])
def artistProfile():

    nameSurname = request.args.get('artist')
    name = nameSurname.split("_")[0]
    surname = nameSurname.split("_")[1]
    
    cursor.execute("SELECT id FROM dbtify.artist WHERE name = '{}' AND surname = '{}' ".format(name, surname))
    artist_id = cursor.fetchone()

    if request.method == "GET": 
        pass

    if request.method == "POST": # If a new song or album is created
        if "genre" in request.form: # If a new album is created
            album_id = request.form["ID"]
            title = request.form["title"]
            genre = request.form["genre"]

            cursor.execute( "SELECT EXISTS(SELECT * FROM dbtify.album WHERE id = '{}')".format(album_id) )

            if cursor.fetchone()[0] > 0: # check if album is already exists
                #return render_template('artistMainPage.html', artist_list = artist_list, name = name, surname=surname, duplicate = True)
                print("album {} already exists".format(id))

            else: # if not exists in db then add it
                cursor.execute("INSERT INTO album (id, genre, title, artist_id) VALUES ('{}', '{}', '{}', '{}');".format(album_id, genre, title, artist_id[0]))
                conn.commit()

        elif "song_album" in request.form: # If a new song is created
            song_id = request.form["ID"]
            title = request.form["title"]
            album = request.form["song_album"]
            contributors = request.form["cont_list"].split("%")[:-1]


            cursor.execute( "SELECT EXISTS(SELECT * FROM dbtify.song WHERE id = '{}' )".format(song_id) )

            if cursor.fetchone()[0] > 0: # check if song is already exists
                #return render_template('artistMainPage.html', artist_list = artist_list, name = name, surname=surname, duplicate = True)
                print("song {} already exists".format(song_id))

            else: # if not exists in db then add it
                cursor.execute("INSERT INTO song (id, title, album) VALUES ('{}', '{}', '{}');".format(song_id, title, album))
                
                conn.commit()

                cursor.execute("INSERT INTO song_artist (song_id, artist_id) VALUES ('{}', '{}');".format(song_id, artist_id[0]))
                
                conn.commit()

                for contributor in contributors: # add contributors
                    cont_name = contributor.split(" ")[0]
                    cont_surname = contributor.split(" ")[1]
                    cursor.execute("SELECT id FROM dbtify.artist WHERE name = '{}' AND surname = '{}';".format(cont_name, cont_surname))
                    cont_id = cursor.fetchone()

                    cursor.execute("INSERT INTO song_artist (song_id, artist_id) VALUES ('{}', '{}');".format(song_id, cont_id[0]))
                    conn.commit()

        elif "deleteAlbum" in request.form: # If an album is deleted
            album_id = request.form["album_id"]
            cursor.execute("DELETE FROM album WHERE id = '{}' ".format(album_id))
            conn.commit()


        elif "deleteSong" in request.form: # If a song is deleted
            song_id = request.form["song_id"]
            print(song_id)
            cursor.execute("DELETE FROM song WHERE id = '{}'".format(song_id))

            conn.commit()

        else:
            print("Neither song nor album with POST method. ERROR!")

    else:
        print("Error! Couldn't get any form request!")
    


    cursor.execute("SELECT * FROM dbtify.album WHERE artist_id = '{}' ".format(artist_id[0]))

    albums = cursor.fetchall()


    cursor.execute("SELECT * FROM dbtify.artist WHERE id NOT IN(SELECT id FROM dbtify.artist WHERE id = {})".format(artist_id[0]))

    artist_list = cursor.fetchall()

    

    cursor.execute("SELECT song_id , title, album  FROM dbtify.song INNER JOIN dbtify.song_artist ON song.id = song_artist.song_id WHERE artist_id = {}".format(artist_id[0]))
    songs = cursor.fetchall()
    
    return render_template('artistProfile.html', name = name, surname = surname, artist_list = artist_list, album_list = albums, song_list = songs)


@app.route("/viewArtist", methods =["POST", "GET"])
def viewArtist():

    listener_info = request.args.get("listener").split("_")

    listener_id = listener_info[0]

    listener_username = listener_info[1]

    artist_id = request.args.get("artist")

    cursor.execute("SELECT * FROM dbtify.album WHERE artist_id = '{}'".format(artist_id))
    
    album_list = cursor.fetchall()

    cursor.execute("SELECT * FROM dbtify.song \
        INNER JOIN dbtify.album ON song.album = album.id \
        INNER JOIN dbtify.song_artist ON song.id = song_artist.song_id \
        INNER JOIN dbtify.artist ON song_artist.artist_id = artist.id \
        WHERE artist.id = '{}'".format(artist_id) )
    
    song_list = cursor.fetchall()

    cursor.execute("SELECT name, surname, id FROM dbtify.artist WHERE id = '{}'".format(artist_id))

    artist_info = cursor.fetchone()

    cursor.execute("call get_contributors('{}', '{}');".format(artist_info[0], artist_info[1])) #stored procedure

    contributors_list = cursor.fetchone()[0].split("#")[:-1]

    cont_dict = {} # parse string into a dict

    for contribution in contributors_list:
        pair = eval(contribution)
        p_song_id = pair[0]
        p_artist_id = pair[1]

        cursor.execute("select id, title from song where id ='{}';".format(p_song_id))

        p_song_info = cursor.fetchone()

        p_song_info = str(p_song_info[0]) + " - " + str(p_song_info[1])

        cursor.execute("select name, surname from artist where id ='{}';".format(p_artist_id))

        p_artist_info = cursor.fetchone()

        p_artist_info = str(p_artist_info[0]) + " " + str(p_artist_info[1])

        if p_song_info in cont_dict:
            cont_dict[p_song_info].append(p_artist_info)
        else:
            cont_dict[p_song_info] = [p_artist_info]

    if request.method == "POST":
        liked_one = ""
        if "album" in request.form:
            liked_album = request.form["album"].split("_")
            liked_album_id = int(liked_album[0])
            liked_album_title = liked_album[1]
            liked_one = liked_album_title


            cursor.execute( "SELECT EXISTS(SELECT * FROM listener_album WHERE album_id = '{}' AND listener_id = '{}')".format(liked_album_id, listener_id) )

            if cursor.fetchone()[0] > 0: # check if album is already liked

                cursor.execute("SELECT id FROM song WHERE album = '{}'".format(liked_album_id))

                song_of_album = cursor.fetchall()

                song_of_album = list(map(list,song_of_album))

                for song in song_of_album:

                    cursor.execute( "SELECT EXISTS(SELECT * FROM listener_song WHERE song_id = '{}' AND listener_id = '{}')".format(song[0], listener_id) )

                    if cursor.fetchone()[0] == 0: # check if song is already liked
                        cursor.execute("INSERT INTO listener_song (listener_id, song_id) VALUES ('{}', '{}');".format(listener_id, song[0]))
                    
                        conn.commit()

                return render_template("viewArtist.html", album_list = album_list, song_list = song_list, id = listener_id, username = listener_username, artist = artist_info, cont_dict = cont_dict, success=False, duplicate = True, liked_one = liked_one)

            else:
                cursor.execute("INSERT INTO listener_album (listener_id, album_id) VALUES ('{}', '{}');".format(listener_id, liked_album_id))
                
                conn.commit()

                """cursor.execute("SELECT id FROM song WHERE album = '{}'".format(liked_album_id))

                song_of_album = cursor.fetchall()

                song_of_album = list(map(list,song_of_album))

                for song in song_of_album: # like the songs of liked album
                    cursor.execute( "SELECT EXISTS(SELECT * FROM listener_song WHERE song_id = '{}' AND listener_id = '{}')".format(song[0], listener_id) )

                    if cursor.fetchone()[0] == 0: # check if song is already liked
                        cursor.execute("INSERT INTO listener_song (listener_id, song_id) VALUES ('{}', '{}');".format(listener_id, song[0]))
                    
                        conn.commit()"""


        elif "song" in request.form:
            liked_song = request.form["song"].split("_")
            liked_song_id = int(liked_song[0])
            liked_song_title = liked_song[1]

            liked_one = liked_song_title

            cursor.execute( "SELECT EXISTS(SELECT * FROM listener_song WHERE song_id = '{}' AND listener_id = '{}')".format(liked_song_id, listener_id) )

            if cursor.fetchone()[0] > 0: # check if song is already liked
                return render_template("viewArtist.html", album_list = album_list, song_list = song_list, id = listener_id, username = listener_username, artist = artist_info, cont_dict = cont_dict, success=False, duplicate = True, liked_one = liked_one)

            else:
                cursor.execute("INSERT INTO listener_song (listener_id, song_id) VALUES ('{}', '{}');".format(listener_id, liked_song_id))
                
                conn.commit()


        else:
            print("")
        
        return render_template("viewArtist.html", album_list = album_list, song_list = song_list, id = listener_id, username = listener_username, artist = artist_info, cont_dict = cont_dict, success=True, duplicate = False, liked_one = liked_one)


    return render_template("viewArtist.html", album_list = album_list, song_list = song_list, id = listener_id, username = listener_username, artist = artist_info, cont_dict = cont_dict, success=False)


@app.route('/albumPage', methods = ["GET","POST"])
def albumPage():
    album_id = request.args.get('album').strip()
    
    cursor.execute("SELECT title, genre FROM dbtify.album WHERE id = {}".format(album_id))
    titleGenre = cursor.fetchone()
    title = titleGenre[0].strip()
    genre = titleGenre[1].strip()

    cursor.execute("SELECT id, title FROM dbtify.song WHERE album = {}".format(album_id))
    songs = cursor.fetchall()

    artist_name = request.args.get("artist").split("_")[0]
    artist_surname = request.args.get("artist").split("_")[1]

    flag = False

    if request.method == "POST":
        if "title" in request.form:
            title = request.form["title"].strip()
            flag = True

        if "genre" in request.form:
            genre = request.form["genre"].strip()
            flag = True
        

        cursor.execute("UPDATE dbtify.album SET title = '{}' , genre = '{}' WHERE id = '{}'".format(title, genre, album_id))
        conn.commit()

    return render_template("album.html", id=album_id, title=title, genre=genre, songs=songs, flag = flag, artist_name= artist_name, artist_surname= artist_surname)


@app.route('/songPage', methods = ["GET", "POST"])
def songPage():
    song_id = request.args.get('song').strip()
    
    artist = request.args.get("artist").strip().split("_")
    artist_name = artist[0]
    artist_surname = artist[1]
    cursor.execute("SELECT id FROM dbtify.artist WHERE name='{}' AND surname='{}'".format(artist_name, artist_surname))
    artist_id = cursor.fetchone()

    cursor.execute("SELECT title, album FROM dbtify.song WHERE id = {}".format(song_id))
    titleAlbum = cursor.fetchone()
    title = titleAlbum[0].strip()
    album_id = titleAlbum[1]

    cursor.execute("SELECT artist_id FROM dbtify.song_artist WHERE song_id = '{}'".format(song_id))
    cont_list = cursor.fetchall()
    first = lambda x: x[0]
    cont_ids = list(map(first, list(cont_list) ) )
    cont_ids.remove(artist_id[0])

    cursor.execute("SELECT * FROM dbtify.artist")

    artist_list = cursor.fetchall()
    artist_list = list(map(list, artist_list))

    artist_list.remove([artist_id[0], str(artist_name), str(artist_surname)])

    artist_ids = map(first, artist_list)

    flag = False

    if request.method == "POST":
        if "title" in request.form:
            title = request.form["title"].strip()
            flag = True

        contributors = request.form["cont_list"].split("%")[:-1]

        cursor.execute("DELETE FROM dbtify.song_artist WHERE song_id = '{}'".format(song_id))
        conn.commit()

        cursor.execute("INSERT INTO song_artist (song_id, artist_id) VALUES ('{}', '{}');".format(song_id, artist_id[0]))
                
        conn.commit()

        for contributor in contributors: # add contributors
            cont_name = contributor.split(" ")[0]
            cont_surname = contributor.split(" ")[1]
            cursor.execute("SELECT id FROM dbtify.artist WHERE name = '{}' AND surname = '{}';".format(cont_name, cont_surname))
            cont_id = cursor.fetchone()

            cursor.execute("INSERT INTO song_artist (song_id, artist_id) VALUES ('{}', '{}');".format(song_id, cont_id[0]))
            conn.commit()


        cursor.execute("UPDATE dbtify.song SET title = '{}' WHERE id = '{}'".format(title, song_id))
        conn.commit()

    return render_template("song.html", id=song_id, title=title, flag = flag, artist_name= artist_name, artist_surname= artist_surname, album=album_id, cont_ids = cont_ids, artists=artist_list)



    return



@app.route('/listener', methods=["POST", "GET"])
def listener():
    cursor.execute("SELECT * FROM dbtify.listener")
    
    listener_list = cursor.fetchall()
    
    username = ""
    email = ""

    if request.method == "POST": # if a new listener is created
        username = request.form["username"]
        email = request.form["email"]

        cursor.execute( "SELECT EXISTS(SELECT * FROM dbtify.listener WHERE username = '{}' OR email = '{}' )".format(username, email) )

        if cursor.fetchone()[0] > 0: # check if listener is already exists
            return render_template('listenerMainPage.html', listener_list = listener_list, username = username, email=email, duplicate = True)

        else: # if not exists in db then add it
            cursor.execute("INSERT INTO dbtify.listener (username, email) VALUES ('{}', '{}');".format(username, email))
            conn.commit()
            #return all the listeners from db
            cursor.execute("SELECT * FROM dbtify.listener")
            listener_list = cursor.fetchall()
            return render_template('listenerMainPage.html', listener_list = listener_list, username = username, email=email, duplicate=False)

    
    return render_template('listenerMainPage.html', listener_list = listener_list)


@app.route('/listenerProfile')
def listenerProfile():

    listener_info = request.args.get("listener").split("_")

    listener_id = listener_info[0]

    listener_username = listener_info[1]

    cursor.execute("SELECT * FROM dbtify.listener_song \
        INNER JOIN dbtify.song ON song.id = listener_song.song_id\
        INNER JOIN dbtify.song_artist ON song_artist.song_id = song.id\
        INNER JOIN dbtify.artist ON song_artist.artist_id = artist.id\
        WHERE listener_id = '{}' order by title".format(listener_id))
    
    liked_songs = cursor.fetchall()

    cursor.execute("SELECT listener_id, username, title, name, surname FROM dbtify.listener_song \
        INNER JOIN dbtify.song ON song.id = listener_song.song_id\
        INNER JOIN dbtify.song_artist ON song_artist.song_id = song.id\
        INNER JOIN dbtify.artist ON song_artist.artist_id = artist.id\
        INNER JOIN dbtify.listener ON listener_song.listener_id = listener.id\
        WHERE NOT listener_id = '{}' order by title".format(listener_id))

    all_liked_songs = cursor.fetchall()

    all_liked_songs = list(map(list, all_liked_songs))

    cursor.execute("SELECT id, username FROM listener WHERE NOT id = '{}'".format(listener_id))

    listener_list = cursor.fetchall()
    listener_list = list(map(list, listener_list))

    
    cursor.execute("select count(listener_song.song_id), artist_id, name, surname from listener_song \
        inner join song_artist on song_artist.song_id = listener_song.song_id \
        inner join artist on artist.id = song_artist.artist_id \
        group by artist_id order by count(listener_song.song_id) DESC;")

    
    n_likes_per_artist = cursor.fetchall()

    n_likes_per_artist = list(map(list, n_likes_per_artist))

    liked_artists = list(artist[1] for artist in n_likes_per_artist)

    cursor.execute("select * from artist order by name asc")

    artists = cursor.fetchall()

    cursor.execute("call songs_per_like()")

    #likes title artist_id name surname
    #song_id title likes artist_id name surname

    songs_per_like = cursor.fetchall()

    songs_per_like = list(map(list, songs_per_like))

    liked_song_ids = list(song[0] for song in songs_per_like)

    cursor.execute("select song.id, song.title, song_artist.artist_id, artist.name, artist.surname from song \
        inner join song_artist on song_artist.song_id = song.id \
        inner join artist on song_artist.artist_id = artist.id\
        order by song.title asc")

    songs = cursor.fetchall()

    print(songs_per_like)
    print(liked_song_ids)
    print(songs)


    return render_template('listenerProfile.html', id = listener_id, username = listener_username, liked_songs = liked_songs, \
        all_liked_songs = all_liked_songs, listener_list = listener_list, \
        likes_per_artist = n_likes_per_artist, artists = artists, liked_artists = liked_artists , songs = songs, songs_per_like = songs_per_like, liked_song_ids = liked_song_ids)

@app.route("/viewAll", methods = ["POST", "GET"])
def viewAll():

    listener_info = request.args.get("listener").split("_")

    listener_id = listener_info[0]

    listener_username = listener_info[1]


    cursor.execute("SELECT * FROM dbtify.artist")
    
    artist_list = cursor.fetchall()

    cursor.execute("SELECT * FROM dbtify.album")
    
    album_list = cursor.fetchall()

    cursor.execute("SELECT * FROM dbtify.song INNER JOIN dbtify.album ON song.album = album.id ")
    
    song_list = cursor.fetchall()

    if request.method == "POST":
        liked_one = ""
        if "song" in request.form:
            liked_song = request.form["song"].split("_")
            liked_song_id = int(liked_song[0])
            liked_song_title = liked_song[1]

            liked_one = liked_song_title

            cursor.execute( "SELECT EXISTS(SELECT * FROM listener_song WHERE song_id = '{}' AND listener_id = '{}')".format(liked_song_id, listener_id) )

            if cursor.fetchone()[0] > 0: # check if song is already liked
                return render_template("viewAll.html", id = listener_id, username = listener_username, artist_list = artist_list, album_list = album_list, song_list = song_list, success=False, duplicate = True, liked_one = liked_one)

            else:
                cursor.execute("INSERT INTO listener_song (listener_id, song_id) VALUES ('{}', '{}');".format(listener_id, liked_song_id))
                
                conn.commit()                

        return render_template("viewAll.html", id = listener_id, username = listener_username, artist_list = artist_list, album_list = album_list, song_list = song_list, success=True, duplicate = False, liked_one = liked_one)

    return render_template("viewAll.html", id = listener_id, username = listener_username, artist_list = artist_list, album_list = album_list, song_list = song_list, success = False)


@app.route("/viewAlbum", methods = ["POST", "GET"])
def viewAlbum():
    listener_info = request.args.get("listener").split("_")
    listener_id = listener_info[0]
    listener_username = listener_info[1]

    album_id = request.args.get("album")

    cursor.execute("select album.id, genre, title, name, surname from album inner join artist on artist.id = album.artist_id where album.id = '{}';".format(album_id))

    album = cursor.fetchone()

    album = list(album)

    cursor.execute("select id, title from song where album = '{}'".format(album_id))

    songs = cursor.fetchall()

    songs = list(map(list, songs))


    if request.method == "POST": # liking album
        liked_album = request.form["album"].split("_")
        liked_album_id = int(liked_album[0])
        liked_album_title = liked_album[1]

        liked_one = liked_album_title

        cursor.execute( "SELECT EXISTS(SELECT * FROM listener_album WHERE album_id = '{}' AND listener_id = '{}')".format(liked_album_id, listener_id) )

        if cursor.fetchone()[0] > 0: # check if album is already liked

            cursor.execute("SELECT id FROM song WHERE album = '{}'".format(liked_album_id)) # check if album contains non-liked songs

            song_of_album = cursor.fetchall()

            song_of_album = list(map(list,song_of_album))

            for song in song_of_album:
                cursor.execute( "SELECT EXISTS(SELECT * FROM listener_song WHERE song_id = '{}' AND listener_id = '{}')".format(song[0], listener_id) )

                if cursor.fetchone()[0] == 0: # check if song is not already liked
                    cursor.execute("INSERT INTO listener_song (listener_id, song_id) VALUES ('{}', '{}');".format(listener_id, song[0]))
                    
                    conn.commit()
            return render_template("viewAlbum.html", id = listener_id, username = listener_username, album = album, songs = songs, like = False, duplicate = True, liked_one = liked_one)

        else:
            cursor.execute("INSERT INTO listener_album (listener_id, album_id) VALUES ('{}', '{}');".format(listener_id, liked_album_id))
                
            conn.commit()

            """cursor.execute("SELECT id FROM song WHERE album = '{}'".format(liked_album_id)) # like all songs of that album

            song_of_album = cursor.fetchall()

            song_of_album = list(map(list,song_of_album))

            for song in song_of_album:
                cursor.execute( "SELECT EXISTS(SELECT * FROM listener_song WHERE song_id = '{}' AND listener_id = '{}')".format(song[0], listener_id) )

                if cursor.fetchone()[0] == 0: # check if song is already liked
                    cursor.execute("INSERT INTO listener_song (listener_id, song_id) VALUES ('{}', '{}');".format(listener_id, song[0]))
                    
                    conn.commit()"""
            return render_template("viewAlbum.html", id = listener_id, username = listener_username, album = album, songs = songs, like = True, liked_one = liked_one)
    

    return render_template("viewAlbum.html", id = listener_id, username = listener_username, album = album, songs = songs, like = False, duplicate = False)

@app.route("/exploreSongs", methods = ["POST", "GET"])
def exploreSongs():

    listener_info = request.args.get("listener").split("_")
    listener_id = listener_info[0]
    listener_username = listener_info[1]

    cursor.execute("select genre from album group by genre;")

    genres = cursor.fetchall()

    cursor.execute("select song.id, song.title, album.genre from song inner join album where song.album = album.id;")

    songs_by_genre = cursor.fetchall()

    searched_songs = []

    if "keyword" in request.args:
        cursor.execute("select id, title from song where title like '%{}%'".format(request.args.get("keyword")))
        searched_songs = cursor.fetchall()

    return render_template("exploreSongs.html", id = listener_id, username = listener_username, genres = genres, songs = songs_by_genre, searched_songs = searched_songs)


if __name__ == "__main__":
    app.config['MYSQL_DATABASE_DB'] = 'dbtify'
    app.run(debug=True)


