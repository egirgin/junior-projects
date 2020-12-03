DROP TRIGGER IF EXISTS dbtify.after_album_delete;
DROP TRIGGER IF EXISTS dbtify.after_song_delete;
DROP TRIGGER IF EXISTS dbtify.after_album_liked;


DELIMITER $$

CREATE TRIGGER after_album_delete
    AFTER DELETE
    ON album FOR EACH ROW
BEGIN
    delete from song 
    where song.album = old.id;
    
    delete from listener_album 
    where listener_album.album_id = old.id;
    
END$$    

DELIMITER ;

DELIMITER $$

CREATE TRIGGER after_song_delete
    AFTER DELETE
    ON song FOR EACH ROW
BEGIN
    delete from listener_song 
    where listener_song.song_id = old.id;
    
    delete from song_artist 
    where song_artist.song_id = old.id;
    
END$$    

DELIMITER ;

DELIMITER $$

CREATE TRIGGER after_album_liked
    AFTER insert
    ON listener_album FOR EACH ROW
BEGIN
	declare no_more_rows boolean default false;
    declare song_id int;
    declare if_liked boolean default false;
    declare temp_num_likes int default 0;
    
    declare songs_of_album cursor for 
    select id from song
    where song.album = new.album_id;
    
    declare continue handler for not found 
	set no_more_rows := true;
    
    open songs_of_album;
    songs: loop
		fetch songs_of_album into song_id;
        
        if no_more_rows then
			set no_more_rows := false;
			close songs_of_album;
			leave songs;
		end if;
        
        select count(*) into temp_num_likes from listener_song 
        where listener_song.song_id = song_id and
        listener_song.listener_id = new.listener_id;
        
        if temp_num_likes = 0 then
			insert into listener_song (listener_id, song_id) values (new.listener_id, song_id);
		end if;
        
    end loop songs;
    
END$$    

DELIMITER ;