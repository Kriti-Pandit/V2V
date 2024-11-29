import cv2
import numpy as np

def pothole_detected(frame):
    # Convert frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Apply Canny edge detection
    edges = cv2.Canny(gray, 50, 150)
    
    # Use contour detection to check if there are any contours (potential potholes)
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    potholes = []
    for contour in contours:
        # Filter contours based on area
        area = cv2.contourArea(contour)
        if area > 2000:  # Adjust this threshold based on your requirements
            # Get bounding box coordinates of the contour
            x, y, w, h = cv2.boundingRect(contour)
            potholes.append((x, y, x + w, y + h))  # Store bounding box coordinates
    
    return potholes

# Open the video file
cap = cv2.VideoCapture(r"C:\Users\chand\Downloads\real-time-object-detection-potholes-2160-ytshorts.savetube.me.mp4")

while True:
    # Read a frame from the video
    ret, frame = cap.read()
    if not ret:
        break

    # Detect potholes
    detected_potholes = pothole_detected(frame)

    # Highlight potholes by drawing rectangles around bounding boxes
    for (x1, y1, x2, y2) in detected_potholes:
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)

    # Display the frame (for visualization, can be removed in actual implementation)
    cv2.imshow('Frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the video capture object and close all windows
cap.release()
cv2.destroyAllWindows()