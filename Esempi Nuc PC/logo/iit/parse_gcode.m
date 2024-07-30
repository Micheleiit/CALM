function [] = parse_gcode()
    fid = fopen('calm_logo.ngc');
    %fid = fopen('cuore.ngc');
    %fid = fopen('dr_tondeur.ngc');
    %fid = fopen('cuore.ngc');
    x = [];
    y = [];
    speed = [];
    while(~feof(fid))
        
        line = fgetl(fid);  % read line excluding newline character
        if(length(line) > 10)
            
            if(strcmp(line(1:3),'G00')  )
                C = strsplit(line);
                x_string = C{2};
                
                if(strcmp(x_string(1),'X'))
                    x = [x, str2double(x_string(2:end))];
                    y_string = C{3};
                    y = [y, str2double(y_string(2:end))];
                    speed = [speed 1];
                end
            end
            
            if(strcmp(line(1:3),'G01')||strcmp(line(1:3),'G02') || strcmp(line(1:3),'G03'))
                C = strsplit(line);
                x_string = C{2};
                
                if(strcmp(x_string(1),'X'))
                    x = [x, str2double(x_string(2:end))];
                    y_string = C{3};
                    y = [y, str2double(y_string(2:end))];
                    speed = [speed 0];
                end
            end
            
            if(false)%strcmp(line(1:3),'A02') || strcmp(line(1:3),'A03'))
                C = strsplit(line);
                
                x_string = C{2};
                x_end = str2double(x_string(2:end));
                
                y_string = C{3};
                y_end = str2double(y_string(2:end));
                
                x_string = C{5};
                x_center = str2double(x_string(2:end));
                
                y_string = C{6};
                y_center = str2double(y_string(2:end));
                
                if(strcmp(line(1:3),'G02'))
                    [x_new, y_new] = rotation(x(end), y(end), x_end, y_end, x(end)+ x_center, y(end) + y_center, -1);
                else
                    [x_new, y_new] = rotation(x(end), y(end), x_end, y_end, x(end)+ x_center, y(end) + y_center, 1);
                end
                x = [x, x_new];
                y = [y, y_new];
                speed = [speed 0];
                
            end
        end
    end
    
    
    
    
    x = x-mean(x);
    y = y-mean(y);
    
    plot_path(x,y,speed);
    save_h_file(x, y, speed);
end

%%Does not work
function [x, y] = rotation(x_start, y_start, x_end, y_end, x_center, y_center, direction)
    rad = sqrt((x_start- x_center)^2 + (y_start- y_center)^2);
    rad2 = sqrt((x_end- x_center)^2 + (y_end- y_center)^2);
    hold on
    scatter(x_start, y_start, 'b');
    scatter(x_end, y_end, 'r');
    scatter(x_center, y_center, 'g');
    
    angle_start0 = -acosd((x_start- x_center)./rad);
    angle_start = cos_quadrants((x_start- x_center)./rad, (y_start- y_center)./rad);
    
    
    x_interp = x_center + rad* cosd(angle_start+4*direction );
    y_interp = y_center + rad* sind(angle_start+4*direction );
    
    dist_to_start = sqrt((x_interp- x_start)^2 + (y_interp- y_start)^2);
    dist_end_to_start = sqrt((x_end- x_start)^2 + (y_end- y_start)^2);
    
    if(dist_to_start > dist_end_to_start)
        x = x_end;
        y = y_end;
    else
        [x_next, y_next] = rotation(x_interp, y_interp, x_end, y_end, x_center, y_center, direction);
        x = [x_interp, x_next];
        y = [y_interp, y_next];
    end
end

function [x, y] = plot_path(x, y, speed)
    
    x = x(1:length(x)-1);
    y = y(1:length(y)-1);
    speed = speed(1:length(speed)-1);
    
    figure
    hold on
    for k= 2:(length(x)-1)
        if(speed(k+1) == 0)
            plot([x(k) x(k+1)], [y(k) y(k+1)], 'k'); axis equal;
        end
        if(speed(k+1) == 1)
            plot([x(k) x(k+1)], [y(k) y(k+1)], 'k--');
        end
    end
end

function angle = cos_quadrants(dx, dy)
    if(dx >= 0 && dy >= 0)
        angle = asind(dy);
    elseif (dx <= 0 && dy >= 0)
            angle = -asind(dy)+ 180;
    elseif (dx <= 0 && dy <= 0)
            angle = asind(dy)+180;
    elseif (dx >= 0 && dy <= 0)
        angle = asind(dy);
    end 
end

function [] = save_h_file(x, y, speed)
%%fileID = fopen('cuore.h','w');
fileID = fopen('calm_logo.h','w');
fprintf(fileID,'/*h file for tracing*/\n\n');

fprintf(fileID,'int32_t pos_x_gcode[%d] PROGMEM = {\n', length(x));

for i = 1:length(x)-1
    fprintf(fileID,'%d, ', floor(x(i)*5) );
    
    if(mod(i, 8) == 0)
        fprintf(fileID,'\n');
    end
end
fprintf(fileID,'%d};\n\n', floor(x(end)));

fprintf(fileID,'int32_t pos_y_gcode[%d] PROGMEM = {\n', length(y));

for i = 1:length(x)-1
    fprintf(fileID,'%d, ', floor(y(i)*5) );
    
    if(mod(i, 8) == 0)
        fprintf(fileID,'\n');
    end
end
fprintf(fileID,'%d};\n\n', floor(y(end)));


fprintf(fileID,'int32_t speed_gcode[%d] PROGMEM = {\n', length(speed));

for i = 1:length(x)-1
    fprintf(fileID,'%d, ', speed(i) );
    
    if(mod(i, 8) == 0)
        fprintf(fileID,'\n');
    end
end
fprintf(fileID,'%d};', speed(end));

fclose(fileID);
end